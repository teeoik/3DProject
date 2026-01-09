#include <cstdio>
#include <exception>

#include "app/Application.h"

int main()
{
    try
    {
        app::Application app;
        return app.run();
    }
    catch (const std::exception& e)
    {
        std::fprintf(stderr, "Fatal error: %s\n", e.what());
        return 1;
    }
    catch (...)
    {
        std::fprintf(stderr, "Fatal error: unknown exception\n");
        return 1;
    }
}
