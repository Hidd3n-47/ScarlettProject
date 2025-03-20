#pragma once

#include "UI/Panel.h"

namespace ScarlettEditor
{

class PropertiesPanel final : public Panel
{
public:
    explicit inline PropertiesPanel(IView* view, const PanelProperties& properties = PanelProperties()) : Panel{ view, properties } { /* Empty */ }
    void Render() override;

};

} // Namespace ScarlettEditor.
