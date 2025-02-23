#pragma once

namespace ScarlEntt
{
typedef uint32_t EntityId;
typedef uint32_t ComponentId;
} // Namespace Entity

namespace Entity
{
constexpr ScarlEntt::EntityId InvalidEntityId = static_cast<ScarlEntt::EntityId>(-1);
} // Namespace Entity

template <typename T, typename W>
using unordered_map = std::unordered_map<T, W>;

template<typename T>
using vector = std::vector<T>;
