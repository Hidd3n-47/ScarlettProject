#pragma once
#include "UI/Panel.h"

namespace ScarlettEditor
{

class ScenePanel final : public Panel
{
public:
    explicit inline ScenePanel(IView* view, const PanelProperties& properties = PanelProperties()) : Panel{ view, properties } { /* Empty */ }

    void Render() override;
private:

};

} // Namespace ScarlettEditor.
