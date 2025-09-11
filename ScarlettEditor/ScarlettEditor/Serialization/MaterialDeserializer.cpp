#include "ScarlettEditorPch.h"
#include "MaterialDeserializer.h"

#include <Core/FileIO/Filepath.h>
#include <ScarlettUtils/Core/Ulid.h>

#include "Editor/EditorManager.h"
#include "Core/AssetManagement/AssetManager.h"

namespace ScarlettEditor
{

void MaterialDeserializer::Deserialize(const std::string& materialName, const ScarlettUtils::XmlDocument& document)
{
    const auto& materialXmlChildren = document.GetRootElement()->GetChildElements();

    EDITOR_ASSERT(materialXmlChildren.size() == 2 && "Material Xml Document has more children than are being processed, should the deserializer be updated?");

    const std::string ulid{ materialXmlChildren[0].GetValue() };
    const Scarlett::Filepath albedo{ std::string{ AssetManager::ASSET_PATH } + materialXmlChildren[1].GetValue() };

    EditorManager::Instance().GetMaterialManager()->EditorCreateMaterial(ScarlettUtils::Ulid{ std::stoull(ulid) }, materialName, albedo);
}

} // Namespace ScarlettEditor.
