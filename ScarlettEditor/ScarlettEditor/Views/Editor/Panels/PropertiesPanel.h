#pragma once

#include "ContextMenu/ContextMenu.h"
#include "UI/Panel.h"

namespace ScarlettEditor
{

class PropertiesPanel final : public Panel
{
public:
    explicit inline PropertiesPanel(IView* view) : Panel{ view, { .title = "Properties" } } { /* Empty */ }

    void Render() override;

private:
    bool mShowContextMenu = false;
    ContextMenu menu;
};

} // Namespace ScarlettEditor.
