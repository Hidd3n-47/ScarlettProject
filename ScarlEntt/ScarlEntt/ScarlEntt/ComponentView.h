#pragma once

#include "RTTI/Property.h"

namespace ScarlEntt
{

class ComponentView
{
public:
    inline explicit ComponentView(ComponentTypeId componentTypeId, const std::function<std::unordered_map<std::string, Property>*()>& getPropertyFunction)
        : mComponentTypeId(std::move(componentTypeId)), mGetPropertyFunction(getPropertyFunction)
    { /* Empty. */ }

    inline ComponentTypeId* GetComponentTypeId() { return &mComponentTypeId; }

    inline std::unordered_map<std::string, Property>* GetProperties() const { return mGetPropertyFunction(); }
private:
    ComponentTypeId mComponentTypeId;
    std::function<std::unordered_map<std::string, Property>* ()> mGetPropertyFunction;

};

} // Namespace ScarlEntt.