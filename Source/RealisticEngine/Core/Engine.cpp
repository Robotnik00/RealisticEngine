#include <chrono>

#include <iostream>

#include "RealisticEngine/Core/Engine.h"


using namespace RealisticEngine::Core;
using namespace std::chrono;

Engine::Engine()
{
  mTime = 0.0;
  mUpdateFrequency = 20.0;
  mMaxFrameRate = 60.0;
  mCurrentState = NULL;
  mRunning = false;
}

void Engine::Loop()
{
  high_resolution_clock::time_point startTime = high_resolution_clock::now();
  high_resolution_clock::time_point currentTime; 
  duration<double> time_span;

  mFrameRate = 0.0;
  double lastUpdateTime = 0;
  double lastRenderTime = 0;

  while(mRunning)
  {
    currentTime = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(currentTime - startTime); 
    mTime = time_span.count(); 
  
    if(mTime - lastUpdateTime > ((double)1.0 / mUpdateFrequency))
    {
      lastUpdateTime = mTime;

      if(mCurrentState != NULL)
      {
        mCurrentState->Update();
      } 
    } 

    currentTime = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(currentTime - startTime);
    mTime = time_span.count();
    double delta = (mTime - lastUpdateTime) * mUpdateFrequency;



    if(delta > 1.0) delta = 1.0;

    if(mTime - lastRenderTime > ((double)1.0/mMaxFrameRate))
    {
      double fps = (1.0/(mTime - lastRenderTime));
      lastRenderTime = mTime;

      mFrameRate += (fps - mFrameRate) / 5.0;

      if(mCurrentState != NULL)
      {
        mCurrentState->Render(delta);
      }
    }
  }
}
