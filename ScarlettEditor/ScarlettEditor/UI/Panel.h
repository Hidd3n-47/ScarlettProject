#pragma once

namespace ScarlettEditor
{

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

    Panel(const Panel&)               = delete;
    Panel(Panel&&)                    = delete;
    Panel& operator=(Panel&&)         = delete;
    Panel& operator=(const Panel&)    = delete;

    void RenderUi();

    [[nodiscard]] inline bool IsFocused() const { return mIsFocused; }
    [[nodiscard]] inline bool IsHovered() const { return mIsHovered; }
protected:
    IView* mView;

    bool mIsFocused = false;
    bool mIsHovered = false;
private:
    PanelProperties mProperties;

    virtual void Render() = 0;

    void Begin() const;
    void End() const;
};

} // Namespace ScarlettEditor.
