#pragma once

#ifdef SCARLETT_EDITOR_DEBUG

#include <ScarlettLogger/Log.h>

#define EDITOR_TLOG(...)      Scarlett::Log::GetLogger()->trace(__VA_ARGS__)      // Trace Log.
#define EDITOR_DLOG(...)      Scarlett::Log::GetLogger()->debug(__VA_ARGS__)      // Debug Log.
#define EDITOR_ILOG(...)      Scarlett::Log::GetLogger()->info(__VA_ARGS__)       // Info Log.
#define EDITOR_WLOG(...)      Scarlett::Log::GetLogger()->warn(__VA_ARGS__)       // Warn Log.
#define EDITOR_ELOG(...)      Scarlett::Log::GetLogger()->error(__VA_ARGS__)      // Error Log.
#define EDITOR_FLOG(...)      Scarlett::Log::GetLogger()->critical(__VA_ARGS__)   // Fatal Log

#define EDITOR_ASSERT(x) assert(x)

#else

#define EDITOR_TLOG(...)
#define EDITOR_DLOG(...)
#define EDITOR_ILOG(...)
#define EDITOR_WLOG(...)
#define EDITOR_ELOG(...)
#define EDITOR_FLOG(...)

#define EDITOR_ASSERT(x)

#endif // SCARLETT_EDITOR_DEBUG.
