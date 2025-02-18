#pragma once

#ifndef ENTRY_POINT_DEFINED
#define ENTRY_POINT_DEFINED

#include "ScarlettTestProject/Core/TestRegistry.h"

extern Scarlett::TestRegistry* Scarlett::CreateTestEnvironment();

inline int main(int argc, char** argv)
{
    Scarlett::TestRegistry* testRegistry = Scarlett::CreateTestEnvironment();

    testRegistry->Init();
    testRegistry->RunTests();

    return 0;
}

#endif // ENTRY_POINT_DEFINED.