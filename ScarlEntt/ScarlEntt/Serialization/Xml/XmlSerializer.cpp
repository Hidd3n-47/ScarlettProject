#include "ScarlEnttpch.h"
#include "XmlSerializer.h"

#include <ranges>
#include <fstream>
#include <unordered_map>

namespace ScarlEntt
{

bool XmlSerializer::Serialize(const XmlDocument& document, const std::string& path)
{
    if (!document.mRootNode)
    {
        return false;
    }

    std::ofstream file(path, std::ios::out);
    if (file.fail())
    {
        return false;
    }

    OutputNode(file, document.mRootNode);

    file.close();
    return true;
}

 XmlDocument XmlSerializer::Deserialize(const std::string& path)
{
    std::ifstream file(path, std::ios::in);

    if (file.fail())
    {
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

            size_t spacePosition = tagOrValue.find(' ');

            const std::string tag = spacePosition == std::string::npos ? tagOrValue : tagOrValue.substr(0, spacePosition);

            XmlNode* newNode = new XmlNode(tag);
            nodes[tagLevel].emplace_back(newNode);

            while(spacePosition != std::string::npos)
            {
                const size_t equalPosition = tagOrValue.find('=', spacePosition);

                // Since attributes are in the form 'attributeName="attributeValue"', add 2 to equals position to get ending double quote.
                const size_t endDoubleQuotePosition = tagOrValue.find('"', equalPosition + 2);
                newNode->AddAttribute(tagOrValue.substr(spacePosition + 1, equalPosition - spacePosition - 1), tagOrValue.substr(equalPosition + 2, endDoubleQuotePosition - equalPosition - 2));

                spacePosition = tagOrValue.find(' ', endDoubleQuotePosition + 1);
            }

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

    std::string attributeString;
    for(auto& [attribute, attributeValue] : node->GetAttributes())
    {
        attributeString += " " + attribute + "=\"" + attributeValue + "\"";
    }

    file << indent << "<" << node->GetTagName() << attributeString << ">\n";

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

} // Namespace ScarlEntt.
