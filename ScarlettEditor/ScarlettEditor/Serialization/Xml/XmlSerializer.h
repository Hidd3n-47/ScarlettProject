#pragma once

#include "XmlDocument.h"

namespace ScarlettEditor
{

// todo add tests for the serializer.
// todo move \c XmlSerializer into a new project - ScarlettUtils?
class XmlSerializer
{
public:
    static bool Serialize(const XmlDocument& document, const std::string& path);
    static XmlDocument Deserialize(const std::string& path);
private:
    enum class XmlNodeLineType : uint8
    {
        OPEN_TAG,
        CLOSE_TAG,
        TAG_VALUE
    };

    static void OutputNode(std::ofstream& file, const XmlNode* node, const int level = 0);

    static XmlNodeLineType GetLineType(const std::string& line, std::string& value);
};

} // Namespace ScarlettEditor.
