#pragma once

#include <cstdint>

typedef uint64_t EntityId;
typedef uint64_t ComponentId;

constexpr EntityId InvalidEntityId = static_cast<EntityId>(1) << 63;

// TODO (Christian) Invalid ID.
// Todo (Christian) max mEntities and component types etc.
