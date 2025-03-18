#pragma once

namespace ScarlettEditor
{
//todo Panels should have a reference to their view's so that they can have variables.
struct PanelProperties
{
    std::string_view title      = "NoTitle";
};

class Panel
{
public:
    explicit inline Panel(const PanelProperties& properties = PanelProperties()) : mProperties{ properties } { /* Empty. */ }
    virtual ~Panel() = default;

    void RenderUi();
private:
    PanelProperties mProperties;

    virtual void Render() = 0;

    void Begin() const;
    void End() const;
};

} // Namespace ScarlettEditor.
