//SDLWrapper.cpp

#include "SDLWrapper.h"

void SDLWrapper::Init()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    m_pWindow = SDL_CreateWindow("OpenGL Homework"
                                           , -1200, 300 //TODO: This spawns the window on my other monitor, change later
                                           , 800, 600
                                           , SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    m_context = SDL_GL_CreateContext(m_pWindow);

    //INIT GLEW
    glewExperimental = GL_TRUE;
    glewInit();
}

void SDLWrapper::Shutdown()
{
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_pWindow);
    SDL_Quit();
}

void SDLWrapper::SwapWindow()
{
    SDL_GL_SwapWindow(m_pWindow);
}