#pragma once

#include <string>

#include <scarlettTypes/Types.h>

namespace ScarlEntt
{

typedef uint32_t EntityId;
typedef uint32_t ComponentId;

struct ComponentTypeId
{
public:
    ComponentTypeId() = default;
    explicit ComponentTypeId(std::string typeId)
        : mTypeId(std::move(typeId))
    { /* Empty. */ }

    [[nodiscard]] inline std::string Type() const { return mTypeId; }
    [[nodiscard]] inline std::string FriendlyName() const
    {
        const size_t lastColonPosition = mTypeId.find_last_of(':');
        return mTypeId.substr(lastColonPosition + 1, mTypeId.length() - lastColonPosition - 1);
    }

    [[nodiscard]] inline bool operator==(const ComponentTypeId& other) const
    {
        return mTypeId == other.mTypeId;
    }
private:
    std::string mTypeId;
};

} // Namespace Entity

namespace Entity
{
constexpr ScarlEntt::EntityId InvalidEntityId       = static_cast<ScarlEntt::EntityId>(-1);
constexpr ScarlEntt::EntityId InvalidComponentId    = static_cast<ScarlEntt::ComponentId>(-1);
} // Namespace Entity

// Hash specialisation for Component Type ID.
template <>
struct std::hash<ScarlEntt::ComponentTypeId> {
    size_t operator()(const ScarlEntt::ComponentTypeId& obj) const noexcept
    {
        return std::hash<std::string>()(obj.Type());
    }
};
