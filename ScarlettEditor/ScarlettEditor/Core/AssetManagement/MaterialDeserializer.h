#pragma once

#include "IAssetDeserializer.h"

namespace ScarlettEditor
{

class MaterialDeserializer //final : public IAssetDeserializer
{
public:
    MaterialDeserializer()              = delete;
    ~MaterialDeserializer()     = delete;

    MaterialDeserializer(const MaterialDeserializer&)               = delete;
    MaterialDeserializer(MaterialDeserializer&&)                    = delete;
    MaterialDeserializer& operator=(MaterialDeserializer&&)         = delete;
    MaterialDeserializer& operator=(const MaterialDeserializer&)    = delete;

    void Deserialize(const ScarlettUtils::XmlDocument& document);
};

} // Namespace ScarlettEditor.