#pragma once

#include <functional>

#include "../../../ScarlettEditor/ScarlettEditor/Src/ScarlettEditorDefines.h"
#include "../../../ScarlettEngine/ScarlettEngine/Src/ScarlettEngineDefines.h"

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
    inline Property(const PropertyType type, ComponentTypeId ownerType, const std::function<std::string(Property*)>& getFunction, const std::function<void(const std::string_view&)>& setFunction)
        : mType(type)
        , mOwnerType(std::move(ownerType))
        , mGetFunction(getFunction)
        , mSetFunction(setFunction)
    { /* Empty. */ }

    inline void GetPropertyValue(std::string_view& value)
    {
        mPropertyValue = mGetFunction(this);
        value = std::string_view{ mPropertyValue };
    }

    [[nodiscard]] inline std::string_view GetPropertyValue() { std::string f; std::string_view value = mGetFunction(this); return value; }
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

    std::function<std::string(Property*)>                mGetFunction;
    std::function<void(const std::string_view&)>     mSetFunction;

};

} // Namespace ScarlEntt.
