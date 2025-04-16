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

    [[nodiscard]] bool IsValid() const;

    T* operator->() const;
    T* operator->();
private:
    EntityId            mEntityId       = Entity::InvalidEntityId;
    ComponentArray<T>*  mComponentArray = nullptr;
};

/*
  ======================================================================================================================================================
                                                                                                                                                        */
template <typename T>
bool ComponentRef<T>::IsValid() const
{
    return mEntityId != Entity::InvalidEntityId && mComponentArray && mComponentArray->IsEntityInComponentArray(mEntityId);
}

template <typename T>
T* ComponentRef<T>::operator->() const
{
    //todo need to ensure this works in debug and stripped out release
#ifdef SCARLENTT_DEBUG
    assert(IsValid());
#endif // SCARLENTT_DEBUG.

    return mComponentArray->GetComponent(mEntityId);
}

template <typename T>
T* ComponentRef<T>::operator->()
{
    //todo need to ensure this works in debug and stripped out release
#ifdef SCARLENTT_DEBUG
    assert(IsValid());
#endif // SCARLENTT_DEBUG.

    return mComponentArray->GetComponent(mEntityId);
}


} // Namespace ScarlEntt.
