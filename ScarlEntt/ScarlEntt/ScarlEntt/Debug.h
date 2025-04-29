#pragma once

#include <map>
#include <functional>


#ifdef DEV_CONFIGURATION

#include "ReflectedTypes.h"

// namespace Scarlett::Component
// {
//
// std::unordered_map<std::string, std::function<void(ScarlEntt::EntityHandle, const std::string&)>> compRegistry;
//
// template <typename T>
// struct ComponentTypeId;
//
// template <uint32_t id>
// struct ComponentFromId;
//
// #define REGISTER_COMPONENT(T)                                      \
// template<>                                                         \
// struct ComponentTypeId<T>                                          \
// { static constexpr uint32_t value = __COUNTER__; };                \
// template<>                                                         \
// struct ComponentFromId<ComponentTypeId<T>::value>                  \
// { using type = T; }\
// static void Register##T() {                                        \
//     componentRegistry[#T] = [](ScarlEntt::EntityHandle handle, const std::string& value) { \
//     handle.AddComponent(e, T::FromValue(value));                   \
//     };                                                             \
//     }                                                                  \
//     __attribute__((constructor)) static void register_##T() { Register##T(); } // Register on load
//
// template<uint32_t id>
// auto GetDefaultComponentFromTypeId(const std::string& value)
// {
//     using Component = typename ComponentFromId<id>::type;
//     return Component::FromValue(value);
// }
// }

#define COMPONENT_SERIALIZATION(...)                                                  \
public:                                                                               \
inline std::map<std::string, ScarlEntt::TypeReflection>* GetSerializedFunction()      \
{ return &mSerializationMap; }                                                        \
private:                                                                              \
std::map<std::string, ScarlEntt::TypeReflection> mSerializationMap = { __VA_ARGS__ }; \
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
