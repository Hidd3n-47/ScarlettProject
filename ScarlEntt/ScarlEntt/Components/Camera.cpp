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
        if (childNode->GetTagName() == "mForwardVector")
        {
            component.SetForwardVector(ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(childNode->GetValue()));
            continue;
        }
        if (childNode->GetTagName() == "mRightVector")
        {
            component.SetRightVector(ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(childNode->GetValue()));
            continue;
        }
        if (childNode->GetTagName() == "mUpVector")
        {
            component.SetUpVector(ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(childNode->GetValue()));
            continue;
        }
        if (childNode->GetTagName() == "mAspectRatio")
        {
            component.SetAspectRatio(ScarlEntt::TypeReflection::GetValueFromTypeString<float>(childNode->GetValue()));
        }
    }

    return component;
}

} // Namespace Scarlett::Component.
