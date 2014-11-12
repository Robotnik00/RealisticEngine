#ifndef __STATE_H
#define __STATE_H

namespace RealisticEngine
{
  namespace Core
  {
    class State
    {
    public:

      virtual void Initialize() = 0;

      /* Called when state is being updated by the Engine */
      virtual void Update() = 0;

     /* 
      * Called when state is being rendered 
      * @delta: value from [0.0,1.0]. Used for interpolation 
      */
      virtual void Render(double delta) = 0;
    };
  }
}


#endif
