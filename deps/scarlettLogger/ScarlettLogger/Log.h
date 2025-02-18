#pragma once

#include <memory>

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
    /**
     * Initialize the logger.
     * @param timeStampAndNameOnLog Enable timestamp and name for each message that is logged, example "20:20:22 Scarlett:".
     * @param loggerName The name of the logger. If@code timeStampAndNameOnLog@endcode is enabled, this is the name after the timestamp.
     */
    static void Init(const bool timeStampAndNameOnLog = true, const std::string& loggerName = "Scarlett");

    /**
     * Get the logger.
     * @return A reference to the logger.
     */
    [[nodiscard]] inline static std::shared_ptr<spdlog::logger>& GetLogger() { return mSpd; }
private:
    static std::shared_ptr<spdlog::logger> mSpd;
};
} // Namespace Scarlett.