#pragma once
#include "UI/Panel.h"

namespace ScarlettEditor
{

class ScenePanel final : public Panel
{
public:
    explicit inline ScenePanel(const PanelProperties& properties = PanelProperties()) : Panel{ properties } { /* Empty */ }

    void Render() override;
private:

};

} // Namespace ScarlettEditor.
