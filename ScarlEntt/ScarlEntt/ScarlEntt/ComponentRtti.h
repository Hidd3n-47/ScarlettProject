#pragma once

#include <string>
// TODO remove if this is not needed as string seems to fix the mangling.
#ifndef COMPONENT_RTTI
#define COMPONENT_RTTI                                                                              \
    static void TypeName(std::string& type)                                                         \
    {                                                                                               \
        std::string funcSig = __FUNCSIG__;                                                          \
        const std::size_t endOfType = funcSig.find("::");                                           \
        const std::size_t startOfType = funcSig.rfind(" ", endOfType);                              \
        type = funcSig.substr(startOfType, endOfType - startOfType);                                \
    }
#endif
