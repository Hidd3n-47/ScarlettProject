#pragma once

#include <ScarlettUtils/Xml/XmlDocument.h>

namespace ScarlettEditor
{

class IAssetDeserializer
{
public:
    IAssetDeserializer() = delete;
    virtual ~IAssetDeserializer();

    IAssetDeserializer(const IAssetDeserializer&)               = delete;
    IAssetDeserializer(IAssetDeserializer&&)                    = delete;
    IAssetDeserializer& operator=(IAssetDeserializer&&)         = delete;
    IAssetDeserializer& operator=(const IAssetDeserializer&)    = delete;

    //virtual auto Deserialize(const ScarlEntt::XmlDocument& document) = 0;
};

} // Namespace ScarlettEditor.