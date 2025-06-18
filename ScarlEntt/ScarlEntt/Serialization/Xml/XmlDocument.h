#pragma once

namespace ScarlEntt
{

class XmlNode;

class XmlDocument
{
friend class XmlSerializer;
public:
    explicit XmlDocument(XmlNode* parentNode)
        : mRootNode(parentNode)
    { /* Empty. */ }
    XmlDocument() = default;
    ~XmlDocument();

    XmlDocument(const XmlDocument&)               = delete;
    XmlDocument(XmlDocument&&)                    = delete;
    XmlDocument& operator=(XmlDocument&&)         = delete;
    XmlDocument& operator=(const XmlDocument&)    = delete;

    inline XmlNode* GetRootNode() const { return mRootNode; }
private:
    XmlNode* mRootNode = nullptr;
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

    //todo add checking to ensure attribute is not present already.
    inline void AddAttribute(const std::string& attribute, const std::string& value) { mAttributes[attribute] = value; }
    inline std::string GetAttribute(const std::string& attribute) const { return mAttributes.at(attribute); }
    inline const unordered_map<std::string, std::string>& GetAttributes() const { return mAttributes; }
private:
    vector<XmlNode*> mChildren;

    std::string mTagName;
    std::string mValue;

    unordered_map<std::string, std::string> mAttributes;
};

} // Namespace ScarlEntt.
