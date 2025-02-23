#pragma once

namespace ScarlEntt
{

class ComponentManager;

/**
 * @class ISystem: An interface for a system that is registered.<br/>
 * A system is something that is used to act on an array of components.<br/>
 * Systems can be registered with different priorities to enforce update order.
 */
class ISystem
{
public:
    ISystem()            = default;
    virtual ~ISystem()   = default;

    ISystem(const ISystem&)             = delete;
    ISystem(ISystem&&)                  = delete;
    ISystem& operator=(ISystem&&)       = delete;
    ISystem& operator=(const ISystem&)  = delete;

    virtual void InitSystem()       = 0;
    virtual void UpdateSystem()     = 0;
    virtual void DestroySystem()    = 0;

protected:
    ComponentManager* mComponentManagerRef;
};

} // Namespace ScarlEntt
