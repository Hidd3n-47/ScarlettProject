#pragma once

#ifndef ENTRY_POINT_DEFINED
#define ENTRY_POINT_DEFINED

#include "ScarlettTestProject/Core/TestRegistry.h"

extern Scarlett::TestRegistry* Scarlett::CreateTestEnvironment();

// Disable "Warning C4008 : 'main': 'inline' attribute ignored" as main has to be declared as 'inline'
// due to being in header file that can be multi-included if not careful.
#pragma warning(push)
#pragma warning(disable:4008)
inline int main(int argc, char** argv)
#pragma warning(pop)
{
    Scarlett::TestRegistry* testRegistry = Scarlett::CreateTestEnvironment();

    testRegistry->Init();
    testRegistry->RunTests();

    return 0;
}

#endif // ENTRY_POINT_DEFINED.
