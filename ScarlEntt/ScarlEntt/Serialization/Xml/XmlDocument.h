#pragma once

namespace ScarlEntt
{

class XmlNode;

class XmlDocument
{
friend class XmlSerializer;
public:
    explicit XmlDocument(XmlNode* parentNode)
        : mParentNode(parentNode)
    { /* Empty. */ }
    XmlDocument() = default;
    ~XmlDocument();

    XmlDocument(const XmlDocument&)               = delete;
    XmlDocument(XmlDocument&&)                    = delete;
    XmlDocument& operator=(XmlDocument&&)         = delete;
    XmlDocument& operator=(const XmlDocument&)    = delete;

private:
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
    explicit inline XmlNode(std::string tag, std::string value)
        : mTagName(std::move(tag)), mValue(std::move(value))
    { /* Empty. */ }

    inline std::string GetTagName() const { return mTagName; }
    inline std::string GetValue()   const { return mValue; }

    inline void AddChildNode(XmlNode* child) { mChildren.push_back(child); }
    inline const vector<XmlNode*>& GetChildren() const { return mChildren; }
private:
    vector<XmlNode*> mChildren;

    std::string mTagName;
    std::string mValue;
};

} // Namespace ScarlEntt.
