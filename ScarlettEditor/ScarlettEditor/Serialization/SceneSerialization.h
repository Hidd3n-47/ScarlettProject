#pragma once

namespace ScarlettEditor
{

class SceneSerialization
{
public:
    static void SerializeCurrentGameScene();
    static void DeserializeCurrentGameScene();

    const static std::string SCENE_FILE_PATH;
};

} // Namespace ScarlettEditor.
