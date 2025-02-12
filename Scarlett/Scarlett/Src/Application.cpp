#include <ScarlettEngine/Core/Engine.h>

#include <ScarlettEngine/Src/EntryPoint.h>

class Application final : public Scarlett::Engine
{
public:
    Application()            = default;
    ~Application() override  = default;

    Application(const Application&)                 = delete;
    Application(Application&&)                      = delete;
    Application& operator=(Application&&)           = delete;
    Application& operator=(const Application&)      = delete;

};

Scarlett::Engine* Scarlett::CreateEngine()
{
    return new Application();
}