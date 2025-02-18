#include <ScarlettTestProject/Src/EntryPoint.h>
#include <ScarlettTestProject/Core/TestRegistry.h>

#include "Tests/EntityTesting.h"
#include "Tests/ComponentTesting.h"
#include "Tests/ComponentTestingWithHandles.h"

namespace Scarlett
{

class ScarlenttTest : public TestRegistry
{
public:
    inline void InitTestSession() override
    {
        mTestSessionName = "ScarlEntt Tests";
    }

    inline void RegisterTests() override
    {
        EntityTesting                   entityTesting(this);
        ComponentTesting                componentTesting(this);
        ComponentTestingWithHandles     componentTestingWithHandles(this);
    }

};

Scarlett::TestRegistry* Scarlett::CreateTestEnvironment()
{
    return new ScarlenttTest();
}

} // Namespace Scarlett.