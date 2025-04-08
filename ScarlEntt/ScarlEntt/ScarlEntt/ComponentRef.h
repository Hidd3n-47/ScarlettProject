#pragma once

#include "ComponentArray.h"

namespace ScarlEntt
{

template <typename T>
class ComponentRef
{
public:
    ComponentRef() = default;
    inline ComponentRef(const EntityId id, ComponentArray<T>* componentArray)
        : mEntityId(id), mComponentArray(componentArray)
    { /* Empty. */ }

    inline void InvalidateRef() { mComponentArray = nullptr; mEntityId = Entity::InvalidEntityId; }
    [[nodiscard]] inline bool IsValid() const { return mEntityId != Entity::InvalidEntityId && mComponentArray; }

    inline T* operator->() const { return mComponentArray->GetComponent(mEntityId); }
private:
    EntityId            mEntityId       = Entity::InvalidEntityId;
    ComponentArray<T>*  mComponentArray = nullptr;
};

} // Namespace ScarlEntt.
