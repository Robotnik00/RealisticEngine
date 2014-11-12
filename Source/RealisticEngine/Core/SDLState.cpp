#include "RealisticEngine/Core/SDLState.h"

using namespace RealisticEngine::Core;

void SDLState::Initialize()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  mWindow = SDL_CreateWindow("",  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL);
  mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
}

void SDLState::Update()
{
  SDL_Event env;
  while(SDL_PollEvent(&env))
  {
    ProcessEvent(env);
  }
}
