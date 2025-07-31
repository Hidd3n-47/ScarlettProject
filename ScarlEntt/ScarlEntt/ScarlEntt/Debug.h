#pragma once

#ifdef DEV_CONFIGURATION

// This include is needed so each component doesn't have to individually include headers.
#include "RTTI/Property.h"
#include "Serialization/SerializationUtils.h"

#define SCARLETT_COMPONENT(X)                                                             \
public:                                                                                   \
inline std::unordered_map<std::string, ScarlEntt::Property>* GetProperties()              \
    { GenerateProperties(); return &mProperties; }                                        \
inline static X DeserializeComponent(const ScarlEntt::XmlNode* node)                      \
    { return ScarlEntt::SerializationUtils::DeserializeComponent<X>(node); }              \
private:                                                                                  \
std::unordered_map<std::string, ScarlEntt::Property> mProperties;                         \
void GenerateProperties();                                                                \
public:


#else // DEV_CONFIGURATION.

#define SCARLETT_COMPONENT(X)

#define COMPONENT_SERIALIZATION(...)

#endif // Else DEV_CONFIGURATION.
