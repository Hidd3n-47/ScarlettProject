#pragma once

#include <imgui/imgui.h>

#include "UI/Panel.h"

namespace ScarlettEditor
{

class IView
{
public:
    IView()             = default;
    virtual ~IView()    = default;

    void Render() const;

    template <typename T>
    void AddPanel();

    template <typename T>
    std::shared_ptr<Panel> GetPanel();
protected:
    std::unordered_map<std::string, std::shared_ptr<Panel>> mViewPanels;
};

inline void IView::Render() const
{
    ImGui::DockSpaceOverViewport();

    bool show = true;
    ImGui::ShowDemoWindow(&show);

    for (const auto& panel : mViewPanels | std::views::values)
    {
        panel->RenderUi();
    }

    // todo move the console into a panel.
    ImGui::Begin("Console");

    ImGui::End();
}

template <typename T>
void IView::AddPanel()
{
    const char* id = typeid(T).name();
    const std::string typeName(id);

    if (mViewPanels.contains(typeName))
    {
        EDITOR_ELOG("Trying to add panel that is already present in current view.");
        return;
    }

    mViewPanels[typeName] = std::make_shared<T>(this);
}

template <typename T>
std::shared_ptr<Panel> IView::GetPanel()
{
    const char* id = typeid(T).name();
    const std::string typeName(id);

    if (!mViewPanels.contains(typeName))
    {
        EDITOR_ELOG("Trying to retrieve panel that is is not present in current view.");
        return std::shared_ptr<T>(nullptr);
    }

    return mViewPanels[typeName];
}

} // Namespace ScarlettEditor.
