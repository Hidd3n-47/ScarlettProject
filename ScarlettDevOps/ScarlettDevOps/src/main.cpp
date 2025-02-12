#include "Application.h"

int main(int argc, char** argv)
{
    Application app;
    app.Init();
    app.Run();
    app.Destroy();

    return 0;
}