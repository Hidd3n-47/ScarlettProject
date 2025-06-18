#pragma once

#include <functional>

namespace ScarlEntt
{

enum class PropertyType : uint8_t
{
    FLOAT,
    STRING,
    VEC3,
    VEC4,
    QUAT,
};

class Property
{
public:
    Property() = default;
    inline Property(const PropertyType type, ComponentTypeId ownerType, const std::function<std::string()>& getFunction, const std::function<void(const std::string_view&)>& setFunction)
        : mType(type)
        , mOwnerType(std::move(ownerType))
        , mGetFunction(getFunction)
        , mSetFunction(setFunction)
    { /* Empty. */ }

    [[nodiscard]] inline std::string GetPropertyValue()         const { return mGetFunction(); }
    inline void SetPropertyValue(const std::string_view& value) const { mSetFunction(value); }

    [[nodiscard]] inline PropertyType GetType()                 const { return mType; }
    [[nodiscard]] inline ComponentTypeId GetOwnerTypeName()     const { return mOwnerType; }

    [[nodiscard]] inline std::string GetTypeAsString() const
    {
        // todo try make constexpr
        if (mType == PropertyType::FLOAT)     return "float";
        if (mType == PropertyType::STRING)    return "string";
        if (mType == PropertyType::VEC3)      return "vec3";
        if (mType == PropertyType::VEC4)      return "vec4";
        if (mType == PropertyType::QUAT)      return "quat";

        return "UNKNOWN";
    }
    std::string mPropertyValue;
private:
    PropertyType        mType;
    ComponentTypeId     mOwnerType;

    std::function<std::string()>                     mGetFunction;
    std::function<void(const std::string_view&)>     mSetFunction;

};

} // Namespace ScarlEntt.
