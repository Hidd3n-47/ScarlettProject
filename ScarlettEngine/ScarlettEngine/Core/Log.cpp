#include "ScarlettEnginePch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Scarlett
{

std::shared_ptr<spdlog::logger> Log::mSpd;

void Log::Init()
{
    mSpd = spdlog::stdout_color_mt("Scarlett");
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("%^[%T] %n: %v%$");
}

} // Namespace Scarlett.