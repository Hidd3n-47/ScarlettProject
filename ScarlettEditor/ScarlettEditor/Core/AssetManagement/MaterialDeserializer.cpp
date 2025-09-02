#include "ScarlettEditorPch.h"
#include "MaterialDeserializer.h"

#include <Core/FileIO/Filepath.h>

#include "Resources/Manager/MaterialManager.h"

namespace ScarlettEditor
{

void MaterialDeserializer::Deserialize(const ScarlettUtils::XmlDocument& document)
{
    //const auto& materialXmlChildren = document.GetRootElement()->GetChildren();

    //EDITOR_ASSERT(materialXmlChildren.size() == 1 && "Material Xml Document has more children than are being processed, should the deserializer be updated?");

    //const std::string ulid{ materialXmlChildren[1]->GetValue() };
    //const Scarlett::Filepath albedo{ materialXmlChildren[0]->GetValue() };

    //Scarlett::MaterialManager::CreateMaterial(ulid::Unmarshal(ulid), albedo);
}

} // Namespace ScarlettEditor.
