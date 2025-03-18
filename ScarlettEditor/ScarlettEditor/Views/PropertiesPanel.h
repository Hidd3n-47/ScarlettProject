#pragma once

#include "UI/Panel.h"

namespace ScarlettEditor
{

class PropertiesPanel final : public Panel
{
public:
    explicit inline PropertiesPanel(const PanelProperties& properties = PanelProperties()) : Panel{ properties } { /* Empty */ }
    void Render() override;

};

} // Namespace ScarlettEditor.
