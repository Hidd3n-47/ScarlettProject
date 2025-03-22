#include <ScarlettTestProject/Src/EntryPoint.h>
#include <ScarlettTestProject/Core/TestRegistry.h>

#include "Tests/EntityTesting.h"
#include "Tests/EntityHandleTesting.h"
#include "Tests/ComponentTesting.h"
#include "Tests/SystemTesting.h"

namespace Scarlett
{

class ScarlEnttTest final : public TestRegistry
{
public:
    inline void InitTestSession() override
    {
        mTestSessionName = "ScarlEntt Tests";
    }

    inline void RegisterTests() override
    {
        EntityTesting           (this);
        EntityHandleTesting     (this);
        ComponentTesting        (this);
        SystemTesting           (this);
    }

};

Scarlett::TestRegistry* Scarlett::CreateTestEnvironment()  // NOLINT(clang-diagnostic-extra-qualification)
{
    return new ScarlEnttTest();
}

} // Namespace Scarlett.
