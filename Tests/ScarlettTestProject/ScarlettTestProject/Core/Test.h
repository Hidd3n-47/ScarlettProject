#pragma once

#include <string_view>

namespace Scarlett
{

/**
 * @class Test
 * A class to represent the data of a unit test.
 */
class Test
{
public:
    /**
     * Create a test.
     * @param testName The name of the test.
     * @param testPassed If the test passed or not.
     */
    inline Test(const std::string_view testName, const bool testPassed)
        : mTestName(testName)
        , mPassed(testPassed)
    { /* Empty. */ }

    ~Test() = default;

    /** Get if the test passed or not. */
    [[nodiscard]] inline bool GetTestPassed() const { return mPassed; }

    /** Get the name of the test. */
    [[nodiscard]] inline std::string_view GetTestName() const { return mTestName; }
private:
    const std::string_view      mTestName;
    const bool                  mPassed;
};

} // Namespace Scarlett.