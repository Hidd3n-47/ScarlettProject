#pragma once

#include "UiControlProperties.h"

namespace ScarlEntt
{
class Property;
} // Namespace ScarlEntt.

namespace ScarlettEditor
{

struct UiButtonControlProperties;

class UiControls
{
public:
    static void RenderAngle(const ScarlEntt::Property& property, const UiControlProperties& controlProperties = {});
    static void RenderVec3PropertyControl(const ScarlEntt::Property& property, const UiControlProperties& controlProperties = {});
    static void RenderVec4PropertyControl(const ScarlEntt::Property& property, const UiControlProperties& controlProperties = {});
    static void RenderMaterialPropertyControl(const ScarlEntt::Property& property, const UiControlProperties& controlProperties = {});
private:
    static void RenderFloatPropertyControl(const std::string& label, const std::string& propertyId, float& value, const UiControlProperties& controlProperties, const UiButtonControlProperties& buttonProperties);
};

} // Namespace ScarlettEditor.
