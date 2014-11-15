#ifndef ACTION_H
#define ACTION_H

#include <string>

namespace RealisticEngine
{
  namespace Renderer
  {
    class UniformVariable;
  }
  namespace Scene
  {
    class Node;

    class Action
    {
    public:
      virtual void PerformAction() = 0;
    };
  }
}
#endif // ACTION_H
