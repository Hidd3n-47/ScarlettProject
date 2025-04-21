#pragma once

#include <map>
#include <functional>

#ifdef DEV_CONFIGURATION

#define COMPONENT_SERIALIZATION(...)                                    \
inline std::map<std::string, std::string>* GetSerializedFunction()      \
{ return &mSerializationMap; }                                          \
private:                                                                \
std::map<std::string, std::string> mSerializationMap = { __VA_ARGS__ };

namespace ScarlEntt
{

struct ComponentView
{
public:
    ComponentView(std::string typeName, const std::function<std::map<std::string, std::string>*()>& serializeCallback)
        : mComponentTypeId(std::move(typeName)), mSerializeCallback(serializeCallback)
    { /* Empty. */ }

    inline std::string GetComponentTypeId() const { return mComponentTypeId; }
    inline std::map<std::string, std::string>* GetSerializedValue() const { return mSerializeCallback(); }

    inline bool operator==(const std::string& componentTypeName) const
    {
        return mComponentTypeId == componentTypeName;
    }
private:
    std::string mComponentTypeId;
    std::function<std::map<std::string, std::string>*()> mSerializeCallback;
};

} // Namespace ScarlEntt.

#else // DEV_CONFIGURATION.

#define COMPONENT_SERIALIZATION(...)

#endif // Else DEV_CONFIGURATION.
