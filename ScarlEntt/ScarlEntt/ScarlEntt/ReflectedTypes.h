#pragma once

#include "Math/Math.h"

#include "Components/SerializationUtils.h"

namespace ScarlEntt
{

enum class ValueType : uint8_t
{
    FLOAT,
    STRING,
    VEC3,
    VEC4,
    QUAT,
    UNKNOWN
};

template <typename T>
struct TypeToValueType;

template <>
struct TypeToValueType<float>               { static constexpr ValueType value = ValueType::FLOAT; };
template <>
struct TypeToValueType<std::string>         { static constexpr ValueType value = ValueType::STRING; };
template <>
struct TypeToValueType<ScarlettMath::Vec3>  { static constexpr ValueType value = ValueType::VEC3; };
template <>
struct TypeToValueType<ScarlettMath::Vec4>  { static constexpr ValueType value = ValueType::VEC4; };
template <>
struct TypeToValueType<ScarlettMath::Quat>  { static constexpr ValueType value = ValueType::QUAT; };


struct TypeReflection
{
public:
    TypeReflection() = default;
    TypeReflection(const ValueType type, std::string value)
        : mType(type), mValue(std::move(value))
    { /* Empty. */ }
    TypeReflection(const ValueType type, void* valuePtr)
        : mType(type), mValueMemoryAddress(valuePtr)
    { /* Empty. */ }

    // inline static TypeReflection Reflect(const float value)                 { return { ValueType::FLOAT, std::to_string(value) }; }
    // inline static TypeReflection Reflect(std::string value)                 { return { ValueType::STRING, std::move(value) }; }
    // inline static TypeReflection Reflect(const ScarlettMath::Vec3& value)   { return { ValueType::VEC3, SerializationUtils::ToString(value) }; }
    // inline static TypeReflection Reflect(const ScarlettMath::Vec4& value)   { return { ValueType::VEC4, SerializationUtils::ToString(value) }; }
    // inline static TypeReflection Reflect(const ScarlettMath::Quat& value)   { return { ValueType::QUAT, SerializationUtils::ToString(value) }; }


    inline static TypeReflection Reflect(float* value)                { return { ValueType::FLOAT, static_cast<void*>(value) }; }
    inline static TypeReflection Reflect(std::string* value)          { return { ValueType::STRING, static_cast<void*>(value) }; }
    inline static TypeReflection Reflect(ScarlettMath::Vec3* value)   { return { ValueType::VEC3, static_cast<void*>(value) }; }
    inline static TypeReflection Reflect(ScarlettMath::Vec4* value)   { return { ValueType::VEC4, static_cast<void*>(value) }; }
    inline static TypeReflection Reflect(ScarlettMath::Quat* value)   { return { ValueType::QUAT, static_cast<void*>(value) }; }

    template <typename T>
    static T GetValueFromTypeString(const std::string& value)
    {
        // constexpr ValueType type = TypeToValueType<T>::value;
        // switch (type)
        // {
        //     case ValueType::FLOAT:
        //     return GetFloatFromString(value);
        //     case ValueType::STRING:
        //     return value;
        //     case ValueType::VEC3:
        //     return GetVec3FromString(value);
        //     case ValueType::VEC4:
        //     return GetVec4FromString(value);
        //     case ValueType::QUAT:
        //     return GetQuatFromString(value);
        //     default:
        //     // error.
        //     break;
        // }
        // return T();
        // // need to assert or break.

        constexpr ValueType type = TypeToValueType<T>::value;
        if constexpr (type == ValueType::FLOAT)
        {
            return GetFloatFromString(value);
        }
        else if constexpr (type == ValueType::STRING)
        {
            return value;
        }
        else if constexpr (type == ValueType::VEC3)
        {
            return GetVec3FromString(value);
        }
        else if constexpr (type == ValueType::VEC4)
        {
            return GetVec4FromString(value);
        }
        else if constexpr (type == ValueType::QUAT)
        {
            return GetQuatFromString(value);
        }
        else
        {
            static_assert(false, "Unsupported type for GetValueFromTypeString.");
            return T();
        }
    }

    inline ValueType          GetType()  const { return mType; }
    inline const std::string& GetValue() const { return mValue; }
    inline std::string GetValueString() const
    {
        switch (mType)
        {
        case ValueType::FLOAT:
            return std::to_string(*reinterpret_cast<float*>(mValueMemoryAddress));
        case ValueType::STRING:
            return /**static_cast<std::string*>(mValueMemoryAddress)*/ "";
        case ValueType::VEC3:
            return SerializationUtils::ToString(*reinterpret_cast<ScarlettMath::Vec3*>(mValueMemoryAddress));
        case ValueType::VEC4:
            return SerializationUtils::ToString(*reinterpret_cast<ScarlettMath::Vec4*>(mValueMemoryAddress));
        case ValueType::QUAT:
            return SerializationUtils::ToString(*reinterpret_cast<ScarlettMath::Quat*>(mValueMemoryAddress));
        default:
            // todo add logging or debug assert/breakpoint here.
            return "unknown";
        }
    }

    inline std::string GetTypeString() const
    {
        switch (mType)
        {
        case ValueType::FLOAT:
            return "float";
        case ValueType::STRING:
            return "string";
        case ValueType::VEC3:
            return "vec3";
        case ValueType::VEC4:
            return "vec4";
        case ValueType::QUAT:
            return "quat";
        default:
            // todo add logging or debug assert/breakpoint here.
            return "unknown";
        }
    }
private:
    ValueType       mType = ValueType::UNKNOWN;
    std::string     mValue;

    void* mValueMemoryAddress;

    static float GetFloatFromString(const std::string& value);
    static ScarlettMath::Vec3 GetVec3FromString(const std::string& value);
    static ScarlettMath::Vec4 GetVec4FromString(const std::string& value);
    static ScarlettMath::Quat GetQuatFromString(const std::string& value);
};

inline float TypeReflection::GetFloatFromString(const std::string& value)
{
    return std::stof(value);
}

inline ScarlettMath::Vec3 TypeReflection::GetVec3FromString(const std::string& value)
{
    const size_t firstCommaPosition     = value.find_first_of(',');
    const size_t secondCommaPosition    = value.find(',', firstCommaPosition + 1);
    //todo assert on positions;
    const std::string value1 = value.substr(0, firstCommaPosition); // fix
    const std::string value2 = value.substr(firstCommaPosition + 1, secondCommaPosition - firstCommaPosition - 2); // fix
    const std::string value3 = value.substr(secondCommaPosition + 1);
    return ScarlettMath::Vec3( std::stof(value1), std::stof(value2), std::stof(value3) );
}

inline ScarlettMath::Vec4 TypeReflection::GetVec4FromString(const std::string& value)
{
    const size_t firstCommaPosition     = value.find_first_of(',');
    const size_t secondCommaPosition    = value.find(',', firstCommaPosition + 1);
    const size_t thirdCommaPosition     = value.find(',', secondCommaPosition + 1);
    //todo assert on positions;
    const std::string value1 = value.substr(0, firstCommaPosition); // fix
    const std::string value2 = value.substr(firstCommaPosition + 1, secondCommaPosition - firstCommaPosition - 2); // fix
    const std::string value3 = value.substr(secondCommaPosition + 1, thirdCommaPosition - secondCommaPosition - 2); // fix
    const std::string value4 = value.substr(thirdCommaPosition + 1);
    return ScarlettMath::Vec4{ std::stof(value1), std::stof(value2), std::stof(value3), std::stof(value4) };
}

inline ScarlettMath::Quat TypeReflection::GetQuatFromString(const std::string& value)
{
    const size_t firstCommaPosition     = value.find_first_of(',');
    const size_t secondCommaPosition    = value.find(',', firstCommaPosition + 1);
    const size_t thirdCommaPosition     = value.find(',', secondCommaPosition + 1);
    //todo assert on positions;
    const std::string value1 = value.substr(0, firstCommaPosition); // fix
    const std::string value2 = value.substr(firstCommaPosition + 1, secondCommaPosition - firstCommaPosition - 2); // fix
    const std::string value3 = value.substr(secondCommaPosition + 1, thirdCommaPosition - secondCommaPosition - 2); // fix
    const std::string value4 = value.substr(thirdCommaPosition + 1);
    return ScarlettMath::Quat{ std::stof(value1), std::stof(value2), std::stof(value3), std::stof(value4) };
}

}
