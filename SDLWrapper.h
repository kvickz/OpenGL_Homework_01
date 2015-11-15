//SDLWrapper.h

#ifndef SDLWRAPPER_H
#define SDLWRAPPER_H

#include "SDL.h"
#include <gl\glew.h>
#include <SDL_opengl.h>

class SDLWrapper
{
private:
    SDL_Window* m_pWindow;
    SDL_GLContext m_context;

public:
    void Init();
    void Shutdown();

    void SwapWindow();
};

#endif // !SDLWRAPPER_H
