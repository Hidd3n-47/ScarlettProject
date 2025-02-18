#include "TestRegistry.h"

#include <scarlettLogger/log.h>

namespace Scarlett
{

void TestRegistry::Init()
{
    Log::Init(false);
    InitTestSession();

    RegisterTests();
}

void TestRegistry::RunTests()
{
    Log::GetLogger()->trace("Total number of categories for test session \"{0}\": {1}", mTestSessionName, mTests.size());
    Log::GetLogger()->trace("| ==========================================================================================");

    uint32_t totalTestsRun = 0;
    for (const auto& [categoryName, tests] : mTests)
    {
        Log::GetLogger()->trace("|  -------------------- \"{0}\" ----------------------", categoryName);

        uint32_t passed = 0;
        uint32_t failed = 0;

        for (size_t i = 0; i < tests.size(); ++i)
        {
            ++totalTestsRun;
            const Test& test = tests[i];

            if (test.GetTestPassed())
            {
                Log::GetLogger()->info("| {0} / {1} | Test Passed: \"{2}\"", i + 1, tests.size(), test.GetTestName());

                ++passed;
            }
            else
            {
                Log::GetLogger()->error("| {0} / {1} | Test FAILED: \"{2}\"", i + 1, tests.size(), test.GetTestName());

                ++failed;
            }
        }

        Log::GetLogger()->trace("| ----------------------------------------------", categoryName);
        Log::GetLogger()->trace("| \tTests run   : {0}", tests.size());
        Log::GetLogger()->trace("| \tTests passed: {0}", passed);
        Log::GetLogger()->trace("| \tTests failed: {0}", failed);

        mPassedTests += passed;
        mFailedTests += failed;
    }

    Log::GetLogger()->trace("| ==========================================================================================\n");

    Log::GetLogger()->debug("> Tests run   : {0}", totalTestsRun);
    Log::GetLogger()->info("> Tests passed: {0}", mPassedTests);
    // Print the text in red if there were any failed tests.
    if(mFailedTests > 0)
    {
        Log::GetLogger()->error("> Tests failed: {0}", mFailedTests);
    }
    else
    {
        Log::GetLogger()->trace("> Tests failed: {0}", mFailedTests);
    }
}

} // Scarlett.