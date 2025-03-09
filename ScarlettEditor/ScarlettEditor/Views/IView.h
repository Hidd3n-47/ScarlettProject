#pragma once

namespace ScarlettEditor
{

class IView
{
public:
    IView() = default;
    virtual ~IView() = default;

    virtual void Render() = 0;
};

} // Namespace ScarlettEditor.
