#pragma once

namespace ScarlEntt
{

class Scene;
class ComponentManager;

// Todo Christian: Is this needed or the best way to do it?
/**
 * @class ISystemProperties: An interface for properties that are to be passed into<br/>
 * the system during initialization. The properties can be used in the implemented <br/>
 * \c InitSystem of the system by using a re-interpret cast to the correct <br/>
 * type of property.
 */
struct ISystemProperties { };

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

    virtual void InitSystem(ISystemProperties* properties = nullptr) = 0;
    virtual void UpdateSystem()     = 0;
    virtual void DestroySystem()    = 0;

protected:
    Scene*              mSceneRef; // Todo Christian: Should the SystemManager be coupled to the scene.
    ComponentManager*   mComponentManagerRef;
};

} // Namespace ScarlEntt
