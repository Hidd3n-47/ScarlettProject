#include "ScarlEnttpch.h"
#include "XmlDocument.h"

namespace ScarlEntt
{

XmlDocument::~XmlDocument()
{
    while (mParentNode && !mParentNode->mChildren.empty())
    {
        XmlNode* node = mParentNode->mChildren.back();
        XmlNode* previous = mParentNode;

        while (!node->mChildren.empty())
        {
            previous = node;
            node = node->mChildren.back();
        }

        previous->mChildren.pop_back();
        delete node;
    }

    delete mParentNode;
}


} // Namespace ScarlEntt.
