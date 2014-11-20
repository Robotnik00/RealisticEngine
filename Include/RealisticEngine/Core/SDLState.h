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
      void Setup(int width, int height);
      int GetWidth();
      int GetHeight();

      virtual void Initialize();

      virtual void Update();
      virtual void ProcessEvent(SDL_Event env) = 0;

      void SetWindowTitle(const char* title);

      SDL_Window*	mWindow;
      SDL_Renderer*	mRenderer;

      int mWidth = 1024;
      int mHeight = 768;
    };
  }
}

#endif // SDLSTATE_H
