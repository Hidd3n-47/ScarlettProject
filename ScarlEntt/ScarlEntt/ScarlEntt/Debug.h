#pragma once

#include <map>
#include <functional>


#ifdef DEV_CONFIGURATION

#include "ReflectedTypes.h"

#define COMPONENT_SERIALIZATION(...)                                                  \
public:                                                                               \
inline std::map<std::string, ScarlEntt::TypeReflection>* GetSerializedFunction()      \
{                                                                                     \
    /* Regenerate the map to ensure it's up-to-date. */                               \
    mTypeReflectionMap = { __VA_ARGS__ };                                             \
    return &mTypeReflectionMap;                                                       \
}                                                                                     \
private:                                                                              \
std::map<std::string, ScarlEntt::TypeReflection> mTypeReflectionMap;                  \
public:

namespace ScarlEntt
{
struct ComponentView
{
public:
    ComponentView(std::string typeName, const std::function<std::map<std::string, TypeReflection>*()>& serializeCallback)
        : mComponentTypeId(std::move(typeName)), mSerializeCallback(serializeCallback)
    { /* Empty. */ }

    inline std::string GetComponentTypeId() const { return mComponentTypeId; }

    inline std::map<std::string, TypeReflection>* GetSerializedValue() const { return mSerializeCallback(); }

    inline bool operator==(const std::string& componentTypeName) const
    {
        return mComponentTypeId == componentTypeName;
    }
private:
    std::string mComponentTypeId;
    std::function<std::map<std::string, TypeReflection>*()> mSerializeCallback;
};

} // Namespace ScarlEntt.

#else // DEV_CONFIGURATION.

#define COMPONENT_SERIALIZATION(...)

#endif // Else DEV_CONFIGURATION.
