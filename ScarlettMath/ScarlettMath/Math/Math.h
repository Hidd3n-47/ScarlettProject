#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace ScarlettMath
{

// TODO: Make these actual classes instead of just using glm's version of it.
typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;

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

} // Namespace ScarlettMath.
