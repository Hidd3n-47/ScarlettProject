#pragma once

#include <vector>
#include <unordered_map>

#include "Test.h"

#define BIND_FUNCTION(fn)[this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Scarlett
{

class TestRegistry
{
public:
    TestRegistry()              = default;
    virtual ~TestRegistry()     = default;

    TestRegistry(const TestRegistry&)               = delete;
    TestRegistry(TestRegistry&&)                    = delete;
    TestRegistry& operator=(TestRegistry&&)         = delete;
    TestRegistry& operator=(const TestRegistry&)    = delete;

    void Init();
    void RunTests();

    virtual void InitTestSession()  = 0;
    virtual void RegisterTests()    = 0;

    inline void AddTestCase(const std::string_view category, const std::string_view testName, const bool testPassed) { mTests[category].emplace_back(testName, testPassed); }

protected:
    std::string_view mTestSessionName;

private:
    std::unordered_map<std::string_view, std::vector<Test>> mTests;

    uint32_t mPassedTests = 0;
    uint32_t mFailedTests = 0;
};

TestRegistry* CreateTestEnvironment();

} // Namespace Scarlett.