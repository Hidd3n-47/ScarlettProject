#pragma once

#include <ScarlettUtils/Xml/XmlDocument.h>

namespace ScarlettEditor
{

class MaterialDeserializer
{
public:
    MaterialDeserializer()              = delete;
    ~MaterialDeserializer()     = delete;

    MaterialDeserializer(const MaterialDeserializer&)               = delete;
    MaterialDeserializer(MaterialDeserializer&&)                    = delete;
    MaterialDeserializer& operator=(MaterialDeserializer&&)         = delete;
    MaterialDeserializer& operator=(const MaterialDeserializer&)    = delete;

    static void Deserialize(const std::string& materialName, const ScarlettUtils::XmlDocument& document);
};

} // Namespace ScarlettEditor.