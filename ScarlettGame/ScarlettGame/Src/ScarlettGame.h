#pragma once

#include <string>

#ifdef SCARLETT_GAME_DEBUG
#define SCARLETT_GAME_API __declspec(dllexport)
#else // SCARLETT_GAME_EXPORT.
#define SCARLETT_GAME_API __declspec(dllimport)
#endif // SCARLETT_GAME_IMPORT.
