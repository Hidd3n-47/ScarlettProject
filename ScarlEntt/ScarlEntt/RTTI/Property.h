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
    inline Property(const PropertyType type, ComponentTypeId ownerType, const std::function<std::string()>& getFunction, const std::function<void(const std::string&)>& setFunction)
        : mType(type)
        , mOwnerType(std::move(ownerType))
        , mGetFunction(getFunction)
        , mSetFunction(setFunction)
    { /* Empty. */ }

    [[nodiscard]] inline std::string GetPropertyValue() const { std::string value = mGetFunction(); return value; }
    inline void SetPropertyValue(const std::string& value) const { mSetFunction(value); }

    [[nodiscard]] inline PropertyType GetType()                 const { return mType; }
    [[nodiscard]] inline ComponentTypeId GetOwnerTypeName()     const { return mOwnerType; }
private:
    PropertyType        mType;
    ComponentTypeId     mOwnerType;

    std::function<std::string()>                mGetFunction;
    std::function<void(const std::string&)>     mSetFunction;
};

} // Namespace ScarlEntt.
