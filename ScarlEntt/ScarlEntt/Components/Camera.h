#pragma once

#include <Math/Math.h>

namespace Scarlett::Component
{

struct Camera
{
public:
    inline void UpdateViewAndProjectionMatrix(const ScarlettMath::Vec3& eyePosition)
    {
        mViewMatrix         = ScarlettMath::LookAt(eyePosition, eyePosition + mForwardVector, mUpVector);
        mProjectionMatrix   = ScarlettMath::Perspective(60.0f, mAspectRatio, 0.1f, 1000.0f);

        mIsDirty = false;
    }

    inline const ScarlettMath::Mat4& GetViewMatrix()        const { return mViewMatrix; }
    inline const ScarlettMath::Mat4& GetProjectionMatrix()  const { return mProjectionMatrix; }

    inline ScarlettMath::Vec3 GetForwardVector()    const { return mForwardVector; }
    inline ScarlettMath::Vec3 GetRightVector()      const { return mRightVector; }
    inline ScarlettMath::Vec3 GetUpVector()         const { return mUpVector; }

    inline float GetAspectRatio() const { return mAspectRatio; }

    inline void SetViewMatrix(const ScarlettMath::Mat4& viewMatrix)         { mViewMatrix = viewMatrix; mIsDirty = true; }
    inline void SetProjectionMatrix(const ScarlettMath::Mat4& projMatrix)   { mProjectionMatrix = projMatrix; mIsDirty = true;}

    inline void SetForwardVector(const ScarlettMath::Vec3& forward) { mForwardVector = forward; mIsDirty = true; }
    inline void SetRightVector(const ScarlettMath::Vec3& right)     { mRightVector = right; mIsDirty = true; }
    inline void SetUpVector(const ScarlettMath::Vec3& up)           { mUpVector = up; mIsDirty = true; }

    inline void SetDirty() { mIsDirty = true; }
    inline void SetClean() { mIsDirty = false; }
    [[nodiscard]] bool IsDirty() const { return mIsDirty; }

    COMPONENT_SERIALIZATION(
        { "forwardVector", "1,1,1" },
        { "rightVector", "1,1,1" },
        { "upVector", "1,1,1" },
        { "aspectRatio", "1.77777777778" })
private:
    bool mIsDirty = true;

    ScarlettMath::Mat4 mViewMatrix{ 1.0f };
    ScarlettMath::Mat4 mProjectionMatrix{ 1.0f };

    ScarlettMath::Vec3 mForwardVector  { 0.0f, 0.0f, -1.0f };
    ScarlettMath::Vec3 mRightVector    { 1.0f, 0.0f,  0.0f };
    ScarlettMath::Vec3 mUpVector       { 0.0f, 1.0f,  0.0f };

    float mAspectRatio = 16.0f / 9.0f;
};

} // Namespace Scarlett::Component.
