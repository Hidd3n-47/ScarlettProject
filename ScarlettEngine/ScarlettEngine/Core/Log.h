#pragma once

#include <spdlog/spdlog.h>

namespace  Scarlett
{

/**
 * @class Log
 * A class used for logging to the debug console.<br/>
 * Ensure that you initialize the class before use.
 */
class Log
{
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetLogger() { return mSpd; }

private:
    static std::shared_ptr<spdlog::logger> mSpd;
};

} // Namespace Scarlett.