#pragma once

#include <functional>
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
     * @param test A function pointer to the test method.
     */
    //inline Test(const std::string_view testName, const bool testPassed)
    inline Test(const std::string_view testName, const std::function<bool()>& test)
        : mTestName(testName)
        , mTest(test)
    { /* Empty. */ }

    /** Get if the test passed or not. */
    //[[nodiscard]] inline bool GetTestPassed() const { return mPassed; }
    [[nodiscard]] inline bool Run() const { return mTest();}

    /** Get the name of the test. */
    [[nodiscard]] inline std::string_view GetTestName() const { return mTestName; }
private:
    std::string_view            mTestName;
    std::function<bool()>       mTest;
};

} // Namespace Scarlett.
