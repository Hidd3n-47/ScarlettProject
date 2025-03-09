#pragma once

namespace ScarlEntt
{
class EntityHandle;
}

namespace ScarlettEditor
{

class SelectionManager
{
public:

    [[nodiscard]] inline ScarlEntt::EntityHandle* GetSelectedEntity() const { return mSelectedEntity; }
    inline void SetSelectedEntity(ScarlEntt::EntityHandle* entity) { mSelectedEntity = entity;}
private:
    ScarlEntt::EntityHandle* mSelectedEntity = nullptr;
};

} // Namespace ScarlettEditor
