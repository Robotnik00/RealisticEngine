#ifndef ACTION_H
#define ACTION_H

namespace RealisticEngine
{
  namespace Scene
  {
    class Action
    {
    public:
      virtual void performAction() = 0;
    };
  }
}
#endif // ACTION_H
