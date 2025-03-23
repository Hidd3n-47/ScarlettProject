#pragma once

#include "UI/Panel.h"

namespace ScarlettEditor
{

class PropertiesPanel final : public Panel
{
public:
    explicit inline PropertiesPanel(IView* view) : Panel{ view, {.title = "Properties" } } { /* Empty */ }

    void Render() override;

};

} // Namespace ScarlettEditor.
