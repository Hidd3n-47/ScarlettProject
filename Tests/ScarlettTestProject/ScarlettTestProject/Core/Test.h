#pragma once

#include <functional>
#include <string_view>

namespace Scarlett
{

class Test
{
public:
    inline Test(const std::string_view testName, const bool testPassed)
        : mTestName(testName)
        , mPassed(testPassed)
    { /* Empty. */ }

    ~Test() = default;

    inline bool GetTestPassed() const { return mPassed; }

    inline std::string_view GetTestName() const { return mTestName; }
private:
    const std::string_view      mTestName;
    const bool                  mPassed;
};

} // Namespace Scarlett.