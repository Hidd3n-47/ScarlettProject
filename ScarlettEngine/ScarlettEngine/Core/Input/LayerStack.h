#pragma once

#include <stdexcept>

#include "Core/Types/WeakRef.h"

#include "Core/Input/Layer.h"
#include "Core/Events/Event.h"

namespace Scarlett
{

/**
 * @typedef Overlay: Define \c Overlay to store a pointer to \c Layer so that we have type safety<br/>
 * @code Ref<Overlay> != Ref<Layer>@endcode and hence you cannot try \c PopLayer(Ref<Overlay>)
 */
typedef struct { Layer* layer; } Overlay;

/**
 * @class LayerStack: Used to process input on a stack of \c Layer objects.<br/>
 * Can attach \c Layer  or \c Overlay .<br/>
 * \c Layer: Layers are processed after \c Overlay . Example is the game layer.<br/>
 * \c Overlay : Overlays are processed before \c Layer . Example is UI layer on top of the game.<br/>
 * @note \c Layer  and \c Overlay  must be __Popped__ off the stack in order of __Pushing__ to the stack.
 */
class LayerStack
{
public:
    LayerStack() = default;
    ~LayerStack();

    LayerStack(const LayerStack&)             = delete;
    LayerStack(LayerStack&&)                  = delete;
    LayerStack& operator=(LayerStack&&)       = delete;
    LayerStack& operator=(const LayerStack&)  = delete;

    /**
     * @brief Push a \c Layer to the layer stack.
     * @see Layer
     * @tparam layer The class of the layer. This class must inherit from the base \c Layer class.
     * @return A reference to the \c Layer added to the layer stack.
     */
    template<typename layer, typename ...Args>
    [[nodiscard]] WeakRef<Layer> PushLayer(Args ...args);

    /**
     * @brief Push an \c Overlay to the overlay stack.
     * @see Layer
     * @tparam overlay The class of the overlay. This class must inherit from the base \c Layer class.
     * @return A reference to the \c Overlay added to the overlay stack.
     */
    template<typename overlay, typename... Args>
    [[nodiscard]] WeakRef<Overlay> PushOverlay(Args ...args);

    /**
     * @brief Pop a \c Layer off the layer stack.
     * @throws std::runtime_error if \c Layer is not the most recently added layer to the stack.
     * @param layer A reference to the layer that is being popped of the layer stack.
     */
    void PopLayer(WeakRef<Layer>& layer);
    /**
     * @brief Pop an \c Overlay off the overlay stack.
     * @throws std::runtime_error if \c Overlay is not the most recently added overlay to the stack.
     * @param overlay A reference to the overlay that is being popped of the overlay stack.
     */
    void PopOverlay(WeakRef<Overlay>& overlay);

    void OnEvent(Event& e) const;
private:
    vector<Layer*>      mLayers;
    vector<Overlay*>    mOverlays;

    uint32 mLayerId             = 0;
};

/*
  ======================================================================================================================================================
                                                                                                                                                        */

inline LayerStack::~LayerStack()
{
    for (const Overlay* overlay : mOverlays)
    {
        delete overlay->layer;
        delete overlay;
    }

    for (const Layer* layer : mLayers)
    {
        delete layer;
    }
}

template<typename layer, typename ...Args>
inline WeakRef<Layer> LayerStack::PushLayer(Args ...args)
{
    mLayers.emplace_back(new layer(std::forward<Args>(args)...));

    mLayers.back()->mId = mLayerId++;
    mLayers.back()->OnAttach();

    return WeakRef<Layer>{ mLayers.back() };
}

template<typename overlay, typename ...Args>
inline WeakRef<Overlay> LayerStack::PushOverlay(Args ...args)
{
    // TODO: (OPT) Is double 'new' more overhead than inheritance.
    mOverlays.emplace_back(new Overlay{ .layer = new overlay(std::forward<Args>(args)...) });

    mOverlays.back()->layer->mId = mLayerId++;
    mOverlays.back()->layer->OnAttach();

    return WeakRef<Overlay>{ mOverlays.back() };
}

inline void LayerStack::PopLayer(WeakRef<Layer>& layer)
{
    // Todo Strip some of the testing in release builds.

    if (!layer.IsRefValid())
    {
        throw std::runtime_error("Attempting to pop invalid layer.");
    }

    if (layer->GetId() != mLayers.back()->GetId())
    {
        throw std::runtime_error("Attempting to pop layer in a different order.");
    }

    layer->OnDetach();
    delete mLayers.back();
    mLayers.pop_back();

    layer.Invalidate();
}

inline void LayerStack::PopOverlay(WeakRef<Overlay>& overlay)
{
    // Todo Strip some of the testing in release builds.
    if (!overlay.IsRefValid())
    {
        throw std::runtime_error("Attempting to pop invalid overlay.");
    }

    if (overlay->layer->GetId() != mOverlays.back()->layer->GetId())
    {
        throw std::runtime_error("Attempting to pop overlay in a different order.");
    }

    overlay->layer->OnDetach();
    delete mOverlays.back()->layer;
    delete mOverlays.back();
    mOverlays.pop_back();

    overlay.Invalidate();
}

inline void LayerStack::OnEvent(Event& e) const
{
    for(auto it = mOverlays.end(); it != mOverlays.begin(); )
    {
        (*--it)->layer->OnEvent(e);

        if(e.IsHandled())
        {
            return;
        }
    }

    for(auto it = mLayers.end(); it != mLayers.begin(); )
    {
        (*--it)->OnEvent(e);

        if(e.IsHandled())
        {
            return;
        }
    }
}

} // Namespace Scarlett.
