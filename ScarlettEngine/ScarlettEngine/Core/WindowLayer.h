#pragma once

#include "Input/Layer.h"

namespace Scarlett
{

class WindowClosedEvent;

class WindowLayer final : public Layer
{
public:
    void OnEvent(Event& e) override;
private:
    static bool OnWindowClose(const WindowClosedEvent& e);
};

} // Namespace Scarlett.
