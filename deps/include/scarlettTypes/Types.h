#pragma once

typedef uint8_t     uint8;
typedef uint16_t    uint16;
typedef uint32_t    uint32;
typedef uint64_t    uint64;

typedef uint32      textureId;

#include <vector>
#include <unordered_map>

template <typename T, typename W>
using unordered_map = std::unordered_map<T, W>;

template<typename T>
using vector = std::vector<T>;
