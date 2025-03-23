#pragma once

#include "UI/Panel.h"

namespace ScarlettEditor
{

class ViewportPanel final : public Panel
{
public:
    explicit inline ViewportPanel(IView* view) : Panel{ view, {.title = "Viewport" } } { /* Empty */ }

    void Render() override;
private:

};

} // Namespace ScarlettEditor.
