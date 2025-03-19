#pragma once

namespace ScarlettEditor
{
//todo Panels should have a reference to their view's so that they can have variables.
struct PanelProperties
{
    std::string_view title = "NoTitle";
};

class IView;

class Panel
{
public:
    explicit inline Panel(IView* view, const PanelProperties& properties = PanelProperties()) : mView{ view }, mProperties { properties } { /* Empty. */ }
    virtual ~Panel() = default;

    void RenderUi();
protected:
    IView* mView;

private:

    PanelProperties mProperties;

    virtual void Render() = 0;

    void Begin() const;
    void End() const;
};

} // Namespace ScarlettEditor.
