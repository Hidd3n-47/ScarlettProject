#include <ScarlettTestProject/Src/EntryPoint.h>
#include <ScarlettTestProject/Core/TestRegistry.h>

#include "Tests/XmlSerializerTests.h"

namespace Scarlett
{

class ScarlettUtilsTest final : public TestRegistry
{
public:
    inline void InitTestSession() override
    {
        mTestSessionName = "Scarlett Utils Tests";
    }

    inline void RegisterTests() override
    {
        XmlSerializerTests(this);
    }

};

Scarlett::TestRegistry* Scarlett::CreateTestEnvironment()  // NOLINT(clang-diagnostic-extra-qualification)
{
    return new ScarlettUtilsTest();
}

} // Namespace Scarlett.
