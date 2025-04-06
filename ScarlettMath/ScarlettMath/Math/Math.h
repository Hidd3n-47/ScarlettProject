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

static inline float Degrees(const float radians)
{
    return glm::degrees(radians);
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

static inline Mat4 LookAt(const Vec3 eye, const Vec3 center, const Vec3 up)
{
    const Vec3 f { Normalize(center - eye) };
    const Vec3 s { Normalize(cross(f, up)) };
    const Vec3 u { Cross(s, f) };

    Mat4 result { 1 };
    result[0][0] =  s.x;
    result[1][0] =  s.y;
    result[2][0] =  s.z;
    result[0][1] =  u.x;
    result[1][1] =  u.y;
    result[2][1] =  u.z;
    result[0][2] = -f.x;
    result[1][2] = -f.y;
    result[2][2] = -f.z;
    result[3][0] = -dot(s, eye);
    result[3][1] = -dot(u, eye);
    result[3][2] =  dot(f, eye);
    return result;
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
