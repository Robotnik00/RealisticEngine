#ifndef SDLSTATE_H
#define SDLSTATE_H


#include <SDL.h>

#include "RealisticEngine/Core/State.h"

namespace RealisticEngine
{
  namespace Core
  {
    /*
     * A engine state that creates an SDL window
     */
    class SDLState : public State
    {
    public:
      SDLState() { mWidth = 1024; mHeight = 768; }
      void Setup(int width, int height);
      int GetWidth();
      int GetHeight();

      virtual void Initialize();

      virtual void Update();
      virtual void ProcessEvent(SDL_Event env) = 0;

      void SetWindowTitle(const char* title);

      SDL_Window*	mWindow;
      SDL_Renderer*	mRenderer;

      int mWidth;
      int mHeight;
    };
  }
}

#endif // SDLSTATE_H
