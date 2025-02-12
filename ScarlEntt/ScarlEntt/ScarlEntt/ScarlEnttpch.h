#pragma once

#include <cassert>
#include <typeinfo>
#include <unordered_map>

#include "ScarlEntt.h"

//Todo (Christian): Move this assert out of this file so that applications that include this file for the types do not have the debug assert.
#ifdef SCARLENTT_DEBUG
#define SCARLENTT_ASSERT(x) assert(x)
#else // IF SCARLETT_DEBUG.
#define SCARLENTT_ASSERT(x) 
#endif // IF NOT SCARLETT_DEBUG.