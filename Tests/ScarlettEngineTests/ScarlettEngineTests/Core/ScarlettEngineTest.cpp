#include <ScarlettTestProject/Src/EntryPoint.h>
#include <ScarlettTestProject/Core/TestRegistry.h>

#include "Tests/LayerStackTesting/LayerStackTesting.h"

namespace Scarlett
{

class ScarlettEngineTest final : public TestRegistry
{
public:
    inline void InitTestSession() override
    {
        mTestSessionName = "ScarlettEngine Tests";
    }

    inline void RegisterTests() override
    {
        LayerStackTesting(this);
    }

};

Scarlett::TestRegistry* Scarlett::CreateTestEnvironment()  // NOLINT(clang-diagnostic-extra-qualification)
{
    return new ScarlettEngineTest();
}

} // Namespace Scarlett.
