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

static float Sqrt(const float number)
{
    return glm::sqrt(number);
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

static bool IsEqualTo(const float lhs, const float rhs)
{
    constexpr float EPSILON = 0.00001f;

    return (abs(lhs - rhs) < EPSILON);
}

static bool IsEqualTo(const Vec3 lhs, const Vec3 rhs)
{
    return (IsEqualTo(lhs.x, rhs.x)) &&
           (IsEqualTo(lhs.y, rhs.y)) &&
           (IsEqualTo(lhs.z, rhs.z));
}

static bool IsEqualTo(const Mat4& lhs, const Mat4& rhs)
{
    //todo look at optimising this for cache efficiency.
    for ( int i { 0 }; i < 4; ++i)
    {
        for ( int j { 0 }; j < 4; ++j)
        {
            if (!IsEqualTo(lhs[i][j], rhs[i][j]))
            {
                return false;
            }
        }
    }
    return true;
}

} // Namespace ScarlettMath.
