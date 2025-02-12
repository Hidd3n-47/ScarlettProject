#pragma once

#ifndef ENTRY_POINT_DEFINED
#define ENTRY_POINT_DEFINED

extern Scarlett::Engine* Scarlett::CreateEngine();

inline int main(int argc, char** argv)
{
    Scarlett::Engine* engine = Scarlett::CreateEngine();

    engine->InitEngine();
    engine->Run();
    engine->DestroyEngine();


    return 0;
}

#endif // ENTRY_POINT_DEFINED.