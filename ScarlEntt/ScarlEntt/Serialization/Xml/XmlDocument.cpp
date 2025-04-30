#include "ScarlEnttpch.h"
#include "XmlDocument.h"

namespace ScarlEntt
{

XmlDocument::~XmlDocument()
{
    while (mRootNode && !mRootNode->mChildren.empty())
    {
        XmlNode* node = mRootNode->mChildren.back();
        XmlNode* previous = mRootNode;

        while (!node->mChildren.empty())
        {
            previous = node;
            node = node->mChildren.back();
        }

        previous->mChildren.pop_back();
        delete node;
    }

    delete mRootNode;
}


} // Namespace ScarlEntt.
