#pragma once

#include <string>

#include <Math/Quat.h>

namespace SerializationUtils
{

[[nodiscard]] static inline std::string ToString(const ScarlettMath::Vec3& vector)
{
    return std::to_string(vector.x) + "," + std::to_string(vector.y) + "," + std::to_string(vector.z);
}

[[nodiscard]] static inline std::string ToString(const ScarlettMath::Vec4& vector)
{
    return std::to_string(vector.x) + "," + std::to_string(vector.y) + "," + std::to_string(vector.z) + "," + std::to_string(vector.w);
}

[[nodiscard]] static inline std::string ToString(const ScarlettMath::Quat& quat)
{
    return ToString(quat.ToVector4());
}

} // Namespace SerializationUtils.
