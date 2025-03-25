#pragma once

#include <ScarlettGameCore/Components/Camera.h>

namespace ScarlettGame
{
struct Camera;
}

namespace ScarlettEditor
{

struct ViewportCamera final : public ScarlettGame::Camera
{
public:
    float viewportWidth     = 1920.0f;
    float viewportHeight    = 1080.0f;

    ScarlettMath::Vec3 forwardVector  { 0.0f, 0.0f, -1.0f };
    ScarlettMath::Vec3 rightVector    { 1.0f, 0.0f,  0.0f };
    ScarlettMath::Vec3 upVector       { 0.0f, 1.0f,  0.0f };

    inline void SetDirty() { mIsDirty = true; }
    [[nodiscard]] bool IsDirty() const { return mIsDirty; }
private:
    bool mIsDirty = true;
};

} // Namespace ScarlettEditor.
