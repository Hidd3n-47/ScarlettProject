#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace ScarlettMath
{

// TODO: Make these actual classes instead of just using glm's version of it.
typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;

typedef glm::mat3 Mat3;
typedef glm::mat4 Mat4;

static inline Mat4 LookAt(const Vec3 eye, const Vec3 center, const Vec3 up)
{
    return glm::lookAt(eye, center, up);
}

static inline float Radians(const float degrees)
{
    return glm::radians(degrees);
}

static inline Mat4 Perspective(const float fovDegrees, const float aspectRatio, const float zNear, const float zFar)
{
    return glm::perspective(Radians(fovDegrees), aspectRatio, zNear, zFar);
}

static float Sin(const float angleRadians)
{
    return glm::sin(angleRadians);
}

static float Cos(const float angleRadians)
{
    return glm::cos(angleRadians);
}

static inline Vec3 Normalize(const Vec3 v)
{
    return glm::normalize(v);
}

static inline Vec3 Cross(const Vec3 v1, const Vec3 v2)
{
    return glm::cross(v1, v2);
}

static inline float Dot(const Vec3 v1, const Vec3 v2)
{
    return glm::dot(v1, v2);
}

static inline Vec3 Rotate(const Vec3 v, const float angleRadians, const Vec3 axis)
{
    return glm::rotate(v, angleRadians, axis);
}

static inline float Clamp(const float value, const float min, const float max)
{
    return glm::clamp(value, min, max);
}

static bool IsEqualTo(const Vec3 v1, const Vec3 v2)
{
    constexpr float EPSILON = 0.000001f;

    return (abs(v1.x - v2.x) < EPSILON) &&
           (abs(v1.y - v2.y) < EPSILON) &&
           (abs(v1.z - v2.z) < EPSILON);
}

static inline Vec3 Rotate(const Vec3 vector, const Vec3 rotation)
{
    const float cX = Cos(Radians(rotation.x));
    const float sX = Sin(Radians(rotation.x));

    const float cY = Cos(Radians(rotation.y));
    const float sY = Sin(Radians(rotation.y));

    const float cZ = Cos(Radians(rotation.z));
    const float sZ = Sin(Radians(rotation.z));

    const Mat3 rotateX{ 1.0f, 0.0f, 0.0f, 0.0f, cX, -sX, 0.0f, sX, cX };
    const Mat3 rotateY{ cY, 0.0f, sY, 0.0f, 1.0f, 0.0f, -sY, 0.0f, cY };
    const Mat3 rotateZ{ cZ, -sZ, 0.0f, sZ, cZ, 0.0f, 0.0f, 0.0f, 1.0f };

    return rotateZ * rotateY * rotateX * vector;
}

class Quat
{
public:
    Quat() = default;
    inline explicit Quat(const float x, const float y, const float z, const float w)
        : mQuat{ x, y, z, w }
        , mYaw   { glm::yaw(mQuat) }
        , mPitch { glm::pitch(mQuat) }
        , mRoll  { glm::roll(mQuat) }
    { /* Empty */ }

    inline explicit Quat(const Vec3 yawPitchRollRotation)
        : Quat(yawPitchRollRotation.x, yawPitchRollRotation.y, yawPitchRollRotation.z)
    { /* Empty */ }

    //todo do we have to optimise to prevent two calls to Radians.
    inline explicit Quat(const float yawDegrees, const float pitchDegrees, const float rollDegrees)
        : mQuat { YawPitchRoll(Radians(yawDegrees), Radians(pitchDegrees), Radians(rollDegrees)) }
        , mYaw(Radians(yawDegrees))
        , mPitch(Radians(pitchDegrees))
        , mRoll(Radians(rollDegrees))
    { /* Empty */ }

    static inline Vec3 RotatePoint(const Vec3 point, const Quat& quat)
    {
        const glm::quat pointQuat(0.0f, point.x, point.y, point.z);
        const glm::quat rotatedQuat = quat.mQuat * pointQuat * glm::conjugate(quat.mQuat);

        return { rotatedQuat.x, rotatedQuat.y, rotatedQuat.z };
    }

    inline void SetYaw(const float yaw)      { mQuat = YawPitchRoll(yaw, mPitch, mRoll); }
    inline void SetPitch(const float pitch)  { mQuat = YawPitchRoll(mYaw, pitch, mRoll); }
    inline void SetRoll(const float roll)    { mQuat = YawPitchRoll(mYaw, mPitch, roll); }

    [[nodiscard]] inline float GetYaw()     const { return mYaw; }
    [[nodiscard]] inline float GetPitch()   const { return mPitch; }
    [[nodiscard]] inline float GetRoll()    const { return mRoll; }
private:
    glm::quat mQuat = glm::identity<glm::quat>();

    float mYaw      = 0.0f;
    float mPitch    = 0.0f;
    float mRoll     = 0.0f;

    static inline glm::quat YawPitchRoll(const float yaw, const float pitch, const float roll)
    {
        const glm::quat yawQuat   = glm::quat(glm::vec3(0.0f, yaw, 0.0f));
        const glm::quat pitchQuat = glm::quat(glm::vec3(pitch, 0.0f, 0.0f));
        const glm::quat rollQuat  = glm::quat(glm::vec3(0.0f, 0.0f, roll));

        return yawQuat * pitchQuat * rollQuat;
    }
};

class BoundingBox
{
public:
    BoundingBox() = default;
    BoundingBox(const Vec3 minimumBound, const Vec3 maximumBound)
        : mMinimumBound{ minimumBound }, mMaximumBound{ maximumBound }
    { /* Empty. */ }

    BoundingBox(const Vec3 minimumBound, const Vec3 maximumBound, const Vec3 yawPitchRollRotation)
        : mMinimumBound{ minimumBound }, mMaximumBound{ maximumBound }, mRotation{ yawPitchRollRotation }
    { /* Empty. */}

    [[nodiscard]] inline Vec3 GetCenter() const { return (mMinimumBound + mMaximumBound) * 0.5f; }

    [[nodiscard]] inline Vec3 GetWorldMinimumBound() const { return Quat::RotatePoint(mMinimumBound, mRotation); }
    [[nodiscard]] inline Vec3 GetWorldMaximumBound() const { return Quat::RotatePoint(mMaximumBound, mRotation); }

    [[nodiscard]] inline Vec3 GetLocalMinimumBound() const { return mMinimumBound; }
    [[nodiscard]] inline Vec3 GetLocalMaximumBound() const { return mMaximumBound; }

    [[nodiscard]] inline const Quat& GetRotation() const { return mRotation; }

    void SetMinimumBound(const Vec3 minimumBound)   { mMinimumBound = minimumBound; }
    void SetMaximumBound(const Vec3 maximumBound)   { mMaximumBound = maximumBound; }
    void SetRotation(const Quat& rotation)          { mRotation = rotation; }
private:
    Vec3 mMinimumBound = { -0.5f, -0.5f, -0.01f };
    Vec3 mMaximumBound = {  0.5f,  0.5f,  0.01f };
    Quat mRotation;
};

class Ray
{
public:
    Ray() = default;
    explicit inline Ray(const Vec3 origin, const Vec3 direction)
        : mOrigin(origin), mDirection(direction)
    { /* Empty.*/ }

    [[nodiscard]] inline Vec3 GetOrigin()       const { return mOrigin; }
    [[nodiscard]] inline Vec3 GetDirection()    const { return mDirection; }
private:
    Vec3 mOrigin    = { 0.0f, 0.0f, 0.0f};
    Vec3 mDirection = { 1.0f, 0.0f, 0.0f};
};

static inline bool RayIntersectsWithBoundingBox(const Ray& ray, const BoundingBox& box)
{
    const Vec3 rayDir    = ray.GetDirection();
    const Vec3 rayOrigin = ray.GetOrigin();
    const Vec3 boxOrigin = box.GetCenter();

    const float tMin = ( rayDir.x * (boxOrigin.x - rayOrigin.x) + rayDir.y * (boxOrigin.y - rayOrigin.y) + rayDir.z * (boxOrigin.z - rayOrigin.z) )
                        / Dot(rayDir, rayDir);

    const Vec3 closestPoint = rayOrigin + tMin * rayDir;

    const Vec3 rotatedMin = box.GetWorldMinimumBound();
    const Vec3 rotatedMax = box.GetWorldMaximumBound();

    if ((rotatedMin.x <= closestPoint.x && closestPoint.x <= rotatedMax.x) &&
        (rotatedMin.y <= closestPoint.y && closestPoint.y <= rotatedMax.y) &&
        (rotatedMin.z <= closestPoint.z && closestPoint.z <= rotatedMax.z))
    {
        return true;
    }

    return false;
}


} // Namespace ScarlettMath.
