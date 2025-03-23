#pragma once

#ifdef SCARLETT_DEBUG

#include <ScarlettLogger/Log.h>

#define SCARLETT_TLOG(...)      Scarlett::Log::GetLogger()->trace(__VA_ARGS__)      // Trace Log.
#define SCARLETT_DLOG(...)      Scarlett::Log::GetLogger()->debug(__VA_ARGS__)      // Debug Log.
#define SCARLETT_ILOG(...)      Scarlett::Log::GetLogger()->info(__VA_ARGS__)       // Info Log.
#define SCARLETT_WLOG(...)      Scarlett::Log::GetLogger()->warn(__VA_ARGS__)       // Warn Log.
#define SCARLETT_ELOG(...)      Scarlett::Log::GetLogger()->error(__VA_ARGS__)      // Error Log.
#define SCARLETT_FLOG(...)      Scarlett::Log::GetLogger()->critical(__VA_ARGS__)   // Fatal Log

#define SCARLETT_ASSERT(x) assert(x)

#else

#define SCARLETT_TLOG(...)
#define SCARLETT_DLOG(...)
#define SCARLETT_ILOG(...)
#define SCARLETT_WLOG(...)
#define SCARLETT_ELOG(...)
#define SCARLETT_FLOG(...)

#define SCARLETT_ASSERT(x)

#endif // SCARLETT_DEBUG
