#pragma once

#include <functional>

#ifdef DEV_CONFIGURATION

#include "ReflectedTypes.h"
#include "Serialization/Xml/XmlDocument.h"

class Property;

#define SCARLETT_COMPONENT(X)                                                             \
public:                                                                                   \
static std::unordered_map<std::string, Property> properties;                              \
static X DeserializeComponent(const ScarlEntt::XmlNode* node);                            \
std::unordered_map<std::string, ScarlEntt::TypeReflection>* GetSerializedFunction();      \
private:                                                                                  \
std::unordered_map<std::string, ScarlEntt::TypeReflection> mTypeReflectionMap;            \
public:

class Property
{
public:
    inline Property(const ScarlEntt::ValueType type, const std::function<std::string(void*)>& getFunction, const std::function<void(void*, const std::string& value)>& setFunction)
        : GetValue(getFunction), SetValue(setFunction), mType(type)
    { }

    [[nodiscard]] inline ScarlEntt::ValueType GetType() const { return mType; }
    [[nodiscard]] inline std::string GetValueAsString(void* instance) const { return GetValue(instance); }

    const std::function<std::string(void*)>                         GetValue;
    const std::function<void(void*, const std::string& value)>      SetValue;
private:
    ScarlEntt::ValueType mType;
};

namespace ScarlEntt
{
struct ComponentView
{
public:
    ComponentView(std::string typeName, const std::function<std::unordered_map<std::string, TypeReflection>*()>& serializeCallback)
        : mComponentTypeId(std::move(typeName)), mSerializeCallback(serializeCallback)
    { /* Empty. */ }

    inline std::string GetComponentTypeId() const { return mComponentTypeId; }

    inline std::unordered_map<std::string, TypeReflection>* GetSerializedValue() const { return mSerializeCallback(); }

    inline bool operator==(const std::string& componentTypeName) const
    {
        return mComponentTypeId == componentTypeName;
    }
private:
    std::string mComponentTypeId;
    std::function<std::unordered_map<std::string, TypeReflection>*()> mSerializeCallback;
};

} // Namespace ScarlEntt.

#else // DEV_CONFIGURATION.

#define SCARLETT_COMPONENT()

#define COMPONENT_SERIALIZATION(...)

#endif // Else DEV_CONFIGURATION.
