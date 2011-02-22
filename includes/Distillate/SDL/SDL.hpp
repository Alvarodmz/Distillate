#ifndef __SDL_HPP__
#define __SDL_HPP__

#ifdef WIN32 
#include "SDL.h" 
#else 
#include "SDL/SDL.h" 
#endif 

namespace Distillate
{
    namespace SDL
    {
        bool initSDL();
    }
}

#endif
