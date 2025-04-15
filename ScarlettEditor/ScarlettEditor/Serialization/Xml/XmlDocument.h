#pragma once

namespace ScarlettEditor
{

class XmlNode;

class XmlDocument
{
friend class XmlSerializer;
public:
    XmlDocument() = default;
    ~XmlDocument();

    XmlDocument(const XmlDocument&)               = delete;
    XmlDocument(XmlDocument&&)                    = delete;
    XmlDocument& operator=(XmlDocument&&)         = delete;
    XmlDocument& operator=(const XmlDocument&)    = delete;

private:
    explicit XmlDocument(XmlNode* parentNode)
        : mParentNode(parentNode)
    { /* Empty. */ }

    XmlNode* mParentNode = nullptr;
};

class XmlNode
{
friend class XmlDocument;
friend class XmlSerializer;
public:
    inline XmlNode() = default;
    explicit inline XmlNode(std::string tag)
        : mTagName(std::move(tag))
    { /* Empty. */ }

    inline std::string GetTagName() const { return mTagName; }
    inline std::string GetValue()   const { return mValue; }
private:
    vector<XmlNode*> mChildren;

    std::string mTagName;
    std::string mValue;
};

} // Namespace ScarlettEditor.
