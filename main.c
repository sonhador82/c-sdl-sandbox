
#include <stdio.h>

#include <SDL2/SDL.h>
// #include <SDL2/SDL_opengles2.h>

void compileShader()
{
}

int main()
{
    SDL_version version;

    SDL_GetVersion(&version);

    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "version: %i.%i.%i", version.major, version.minor, version.patch);

    int result = SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDL_Window *wnd;

    wnd = SDL_CreateWindow("Simple Window", 10, 10, 600, 400, SDL_WINDOW_OPENGL);
    if (wnd == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cant create opengGL window");
        exit(1);
    }

    SDL_Event evt;
    while (SDL_WaitEvent(&evt))
    {
        if (evt.type == SDL_QUIT)
        {
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Exiting...");
            SDL_Quit();
        }
        if (evt.type == SDL_MOUSEBUTTONDOWN)
        {
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Clicked...");
        }
    }

    printf("error: %i\n", result);
    exit(1);
}
