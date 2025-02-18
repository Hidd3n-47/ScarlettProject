#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Scarlett
{

std::shared_ptr<spdlog::logger> Log::mSpd;

void Log::Init(const bool timeStampAndNameOnLog /* = true */, const std::string& loggerName /* = "Scarlett" */)
{
    mSpd = spdlog::stdout_color_mt(loggerName);
    spdlog::set_level(spdlog::level::trace);

    if (timeStampAndNameOnLog)
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
    }
    else
    {
        spdlog::set_pattern("%^%v%$");
    }
}

} // Namespace Scarlett.