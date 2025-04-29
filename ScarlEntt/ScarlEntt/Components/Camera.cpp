#include "ScarlEnttpch.h"
#include "Camera.h"

#include <Math/Math.h>

#include "Serialization/Xml/XmlDocument.h"

namespace Scarlett::Component
{

Camera Camera::DeserializeComponent(const ScarlEntt::XmlNode* node)
{
    Camera component;
    //todo assert for children size.
    for (const ScarlEntt::XmlNode* childNode : node->GetChildren())
    {
        if (childNode->GetTagName() == "forwardVector")
        {
            component.SetForwardVector(ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(childNode->GetValue()));
            continue;
        }
        if (childNode->GetTagName() == "rightVector")
        {
            component.SetRightVector(ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(childNode->GetValue()));
            continue;
        }
        if (childNode->GetTagName() == "upVector")
        {
            component.SetUpVector(ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(childNode->GetValue()));
            continue;
        }
        if (childNode->GetTagName() == "aspectRatio")
        {
            component.SetAspectRatio(ScarlEntt::TypeReflection::GetValueFromTypeString<float>(childNode->GetValue()));
        }
    }

    return component;
}

} // Namespace Scarlett::Component.
