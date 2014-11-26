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

    /*
     * an abstract callback from a node update function
     * called once for every node it is added to every frame
     */
    class Action
    {
    public:
      virtual void PerformAction() = 0;
    };
  }
}
#endif // ACTION_H
