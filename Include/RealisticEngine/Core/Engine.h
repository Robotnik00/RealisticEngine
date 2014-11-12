#ifndef __ENGINE_H
#define __ENGINE_H

#include "RealisticEngine/Core/State.h"

namespace RealisticEngine
{
  namespace Core
  {
    class Engine
    {
    public:
      Engine();
     
      /* Control Engine */ 
      void Stop() { mRunning = false; }
      void Start() { mRunning = true; Loop(); } 
 
      /* The current state that the engine is in */ 
      void SetState(State* state) { mCurrentState = state; }
      State* GetState() { return mCurrentState; }

      /* Number of times the current state is updated per second */      
      void SetUpdateFrequency(double freq) { mUpdateFrequency = freq; }
      double GetUpdateFrequency() { return mUpdateFrequency; } 

      /* Number of times the current state is rendered per second */
      void SetMaxFrameRate(double maxFPS) { mMaxFrameRate = maxFPS; }  
      double GetFrameRate() { return mFrameRate; }

      /* Time since engine started in seconds */
      double GetTime() { return mTime; } 

    protected:
           
      /* Loop that processes the state of the Engine */ 
      void Loop();
      
      double mUpdateFrequency;
      double mMaxFrameRate;
      double mFrameRate;
      double mTime;
      bool mRunning;
      

      State* mCurrentState;


    };
  }
}




#endif
