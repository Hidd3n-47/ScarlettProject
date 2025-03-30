#include <ScarlettTestProject/Src/EntryPoint.h>
#include <ScarlettTestProject/Core/TestRegistry.h>

#include "Tests/QuaternionTests.h"
#include "Tests/BoundingBoxIntersectionTests.h"

namespace Scarlett
{

class ScarlettMathTest final : public TestRegistry
{
public:
    inline void InitTestSession() override
    {
        mTestSessionName = "ScarlettMath Tests";
    }

    inline void RegisterTests() override
    {
        QuaternionTests(this);
        BoundingBoxIntersectionTests(this);
    }

};

Scarlett::TestRegistry* Scarlett::CreateTestEnvironment()  // NOLINT(clang-diagnostic-extra-qualification)
{
    return new ScarlettMathTest();
}

} // Namespace Scarlett.
