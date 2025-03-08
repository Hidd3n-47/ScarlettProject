#pragma once

#ifdef SCARLETT_EDITOR_EXPORT
#define EDITOR_API __declspec(dllexport)
#else // Not SCARLETT_EDITOR_EXPORT.
#define EDITOR_API __declspec(dllimport)
#endif // SCARLETT_EDITOR_IMPORT.
