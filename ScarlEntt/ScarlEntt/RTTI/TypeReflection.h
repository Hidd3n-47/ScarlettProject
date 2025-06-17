#pragma once

#include <string>

#include <Math/Quat.h>

namespace ScarlEntt
{

class TypeReflection
{
public:
    template <typename T>
    [[nodiscard]] static std::string GetStringFromValue(const T value);

    template <typename T>
    static void SetValueFromString(T& value, const std::string_view& stringValue);
};

/*
  ======================================================================================================================================================
                                                                                                                                                        */

// --------- Gets. ---------
template <typename T>
inline std::string TypeReflection::GetStringFromValue(const T value)
{
    throw std::runtime_error("Failed to get reflection value for type: " + std::string{ typeid(T).name() });
}

template <>
inline std::string TypeReflection::GetStringFromValue<float>(const float value)
{
    return std::to_string(value);
}

template <>
inline std::string TypeReflection::GetStringFromValue<std::string>(const std::string value)
{
    return std::string{ value };
}

template <>
inline std::string TypeReflection::GetStringFromValue<ScarlettMath::Vec3>(const ScarlettMath::Vec3 value)
{
    return std::to_string(value.x) + "," + std::to_string(value.y) + "," + std::to_string(value.z);
}

template <>
inline std::string TypeReflection::GetStringFromValue<ScarlettMath::Vec4>(const ScarlettMath::Vec4 value)
{
    return std::to_string(value.x) + "," + std::to_string(value.y) + "," + std::to_string(value.z) + "," + std::to_string(value.w);
}

template <>
inline std::string TypeReflection::GetStringFromValue<ScarlettMath::Quat>(const ScarlettMath::Quat value)
{
    return GetStringFromValue<ScarlettMath::Vec4>(value.ToVector4());
}

// --------- Sets. ---------
template <typename T>
inline void TypeReflection::SetValueFromString(T& value, const std::string_view& stringValue)
{
    throw std::runtime_error("Failed to get reflection value for type: " + std::string{ typeid(T).name() });
}

template <>
inline void TypeReflection::SetValueFromString<float>(float& value, const std::string_view& stringValue)
{
    const std::string str = std::string{ stringValue };
    value = std::stof(str);
}

template <>
inline void TypeReflection::SetValueFromString<std::string>(std::string& value, const std::string_view& stringValue)
{
    value = std::string{ stringValue };
}

template <>
inline void TypeReflection::SetValueFromString<ScarlettMath::Vec3>(ScarlettMath::Vec3& value, const std::string_view& stringValue)
{
    const size_t firstCommaPosition = stringValue.find_first_of(',');
    const size_t secondCommaPosition = stringValue.find(',', firstCommaPosition + 1);
    //todo assert on positions;
    const std::string value1 = std::string{ stringValue.substr(0, firstCommaPosition) };
    const std::string value2 = std::string{ stringValue.substr(firstCommaPosition + 1, secondCommaPosition - firstCommaPosition - 2) };
    const std::string value3 = std::string{ stringValue.substr(secondCommaPosition + 1) };

    
    value = ScarlettMath::Vec3(std::stof(value1), std::stof(value2), std::stof(value3));
}

template <>
inline void TypeReflection::SetValueFromString<ScarlettMath::Vec4>(ScarlettMath::Vec4& value, const std::string_view& stringValue)
{
    const size_t firstCommaPosition = stringValue.find_first_of(',');
    const size_t secondCommaPosition = stringValue.find(',', firstCommaPosition + 1);
    const size_t thirdCommaPosition = stringValue.find(',', secondCommaPosition + 1);
    //todo assert on positions;
    const std::string value1 = std::string{ stringValue.substr(0, firstCommaPosition) };
    const std::string value2 = std::string{ stringValue.substr(firstCommaPosition + 1, secondCommaPosition - firstCommaPosition - 2) };
    const std::string value3 = std::string{ stringValue.substr(secondCommaPosition + 1, thirdCommaPosition - secondCommaPosition - 2) };
    const std::string value4 = std::string{ stringValue.substr(thirdCommaPosition + 1) };

    value = ScarlettMath::Vec4{ std::stof(value1), std::stof(value2), std::stof(value3), std::stof(value4) };
}

template <>
inline void TypeReflection::SetValueFromString<ScarlettMath::Quat>(ScarlettMath::Quat& value, const std::string_view& stringValue)
{
    ScarlettMath::Vec4 quatAsVec4{};
    SetValueFromString(quatAsVec4, stringValue);

    value = ScarlettMath::Quat{ quatAsVec4 };
}

} // Namespace ScarlEntt.
