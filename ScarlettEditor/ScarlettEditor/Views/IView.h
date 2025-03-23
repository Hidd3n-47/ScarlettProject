#pragma once

namespace ScarlettEditor
{

class IView
{
public:
    IView()             = default;
    virtual ~IView()    = default;

    virtual void Render()       = 0;

    // todo move this into the panels.
    [[nodiscard]] inline  bool IsFocused() const { return mInFocus; }
    [[nodiscard]] inline  bool IsHovered() const { return mIsHovered; }
protected:
    bool mInFocus   = false;
    bool mIsHovered = false;
};

} // Namespace ScarlettEditor.
