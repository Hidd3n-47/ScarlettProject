#pragma once

#include <string>
#include <unordered_map>

namespace ScarlEntt
{
typedef uint32_t uint32;

typedef uint32_t EntityId;
typedef uint32_t ComponentId;

typedef std::string ComponentTypeId;
} // Namespace Entity

namespace Entity
{
constexpr ScarlEntt::EntityId InvalidEntityId       = static_cast<ScarlEntt::EntityId>(-1);
constexpr ScarlEntt::EntityId InvalidComponentId    = static_cast<ScarlEntt::ComponentId>(-1);
} // Namespace Entity

template <typename T, typename W>
using unordered_map = std::unordered_map<T, W>;

template<typename T>
using vector = std::vector<T>;
