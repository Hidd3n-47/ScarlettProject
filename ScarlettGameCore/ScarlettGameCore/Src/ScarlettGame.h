#pragma once

#include <string>

#ifdef SCARLETT_GAME_CORE_EXPORT
#define SCARLETT_GAME_CORE_API __declspec(dllexport)
#else // SCARLETT_GAME_CORE_EXPORT.
#define SCARLETT_GAME_CORE_API __declspec(dllimport)
#endif // SCARLETT_GAME_CORE_IMPORT.
