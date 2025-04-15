#include "ScarlettEditorPch.h"
#include "XmlSerializer.h"

namespace ScarlettEditor
{

bool XmlSerializer::Serialize(const XmlDocument& document, const std::string& path)
{
    if (!document.mParentNode)
    {
        EDITOR_FLOG("Failed to serialize an xml document with no parent node.");
        return false;
    }

    std::ofstream file(path, std::ios::out);
    if (file.fail())
    {
        EDITOR_FLOG("Failed to serialize file '{0}'", path);
        return false;
    }

    OutputNode(file, document.mParentNode);

    file.close();
    return true;
}

 XmlDocument XmlSerializer::Deserialize(const std::string& path)
{
    std::ifstream file(path, std::ios::in);

    if (file.fail())
    {
        EDITOR_FLOG("Failed to deserialize file '{0}'", path);
        return {};
    }

    std::unordered_map<uint32, vector<XmlNode*>> nodes;
    uint32 tagLevel = 0;

    std::string line;
    while (std::getline(file, line))
    {
        std::string tagOrValue;

        const XmlNodeLineType nextLineType = GetLineType(line, tagOrValue);

        if (nextLineType == XmlNodeLineType::OPEN_TAG)
        {
            ++tagLevel;

            XmlNode* newNode = new XmlNode(tagOrValue);
            nodes[tagLevel].emplace_back(newNode);

            if (tagLevel > 1)
            {
                nodes[tagLevel - 1].back()->mChildren.emplace_back(newNode);
            }

            continue;
        }

        if (nextLineType == XmlNodeLineType::TAG_VALUE)
        {
            nodes[tagLevel].back()->mValue = tagOrValue;
            continue;
        }

        if (nextLineType == XmlNodeLineType::CLOSE_TAG)
        {
            --tagLevel;
        }
    }

    file.close();


    return XmlDocument { nodes[1].empty() ? nullptr : nodes[1].back() };
}

void XmlSerializer::OutputNode(std::ofstream& file, const XmlNode* node, const int level /* = 0 */)
{
    const std::string indent(static_cast<size_t>(level), '\t');
    const std::string innerTagIndent = indent + "\t";

    file << indent << "<" << node->GetTagName() << ">\n";

    if (node->mChildren.empty())
    {
        file << innerTagIndent << node->GetValue() << "\n";
    }

    for (size_t child { 0 }; child < node->mChildren.size(); ++child)
    {
        OutputNode(file, node->mChildren[child], level + 1);
    }

    file << indent << "</" << node->GetTagName() << ">\n";
}

XmlSerializer::XmlNodeLineType XmlSerializer::GetLineType(const std::string& line, std::string& value)
{
    const size_t start = line.find('<');
    const size_t end = line.find('>');

    if (start != std::string::npos && end != std::string::npos && start < end)
    {
        value = line.substr(start + 1, end - start - 1);

        if (value[0] == '/')
        {
            value = value.substr(1, value.size() - 2);
            return XmlNodeLineType::CLOSE_TAG;
        }

        return XmlNodeLineType::OPEN_TAG;
    }

    constexpr char toRemove = '\t';

    auto filtered = line | std::views::filter([toRemove](const char c) { return c != toRemove; });
    value = { filtered.begin(), filtered.end() };

    return XmlNodeLineType::TAG_VALUE;
}

} // Namespace ScarlettEditor.
