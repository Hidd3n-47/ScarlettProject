#pragma once

namespace Scarlett
{

class Event;

/**
 * @class Layer: Layer is a level which input events can be processed on. <br/>
 * Layers can be added either as: \c Layers or \c Overlays. <br/>
 * \c Overlays will always be processed before \c Layers.
 */
class Layer
{
friend class LayerStack;
public:
    Layer(const Layer&)             = delete;
    Layer(Layer&&)                  = delete;
    Layer& operator=(Layer&&)       = delete;
    Layer& operator=(const Layer&)  = delete;

    /**
     * @brief Method called when a layer is added to the stack.
     */
    virtual void OnAttach() { }

    /**
     * @brief Method called when a layer is removed to the stack.
     */
    virtual void OnDetach() { }

    /**
     * @brief Method called when an event occurs.
     */
    virtual void OnEvent(Event& e) { }

    /**
     * @brief Get the ID of the Layer.
     * @return The ID of the layer.
     */
    [[nodiscard]] uint32 GetId()    const { return mId; }
protected:
    Layer()                         = default;
    virtual ~Layer()                = default;

    uint32 mId          = -1;
};

} // Namespace Scarlett.
