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
        EntityTesting           entityTesting(this);
        EntityHandleTesting     entityHandleTesting(this);
        ComponentTesting        componentTesting(this);
        SystemTesting           systemTesting(this);
    }

};

Scarlett::TestRegistry* Scarlett::CreateTestEnvironment()
{
    return new ScarlEnttTest();
}

} // Namespace Scarlett.
