#ifndef SDLSTATE_H
#define SDLSTATE_H

#include <SDL.h>

#include "RealisticEngine/Core/State.h"

namespace RealisticEngine
{
  namespace Core
  {
    class SDLState : public State
    {
    public:

      virtual void Initialize();

      virtual void Update();
      virtual void ProcessEvent(SDL_Event env) = 0;



      SDL_Window*	mWindow;
      SDL_Renderer*	mRenderer;
    };
  }
}

#endif // SDLSTATE_H
