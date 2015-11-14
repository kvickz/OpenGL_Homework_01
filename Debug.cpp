//Debug.cpp

#include "Debug.h"

#include "SDL.h"

void Debug::Log(std::string message)
{
    SDL_Log(message.c_str());
}