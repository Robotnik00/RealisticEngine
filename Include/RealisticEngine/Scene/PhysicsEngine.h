#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <glm/glm.hpp>

#include "RealisticEngine/Scene/Geomerty.h"

namespace RealisticEngine
{
  namespace Scene
  {
    class Node;



    class PhysicsMaterial
    {
    public:
      double mStaticFriction;
      double mDynamicFriction;
      double mRestitution;
      double mDensity;
    };

    class PhysicsEngine
    {
    public:
      typedef enum
      {
        RIGID_DYNAMIC = 0,
        RIGID_STATIC
      } ObjectType;

      virtual void Setup(double gravity) = 0;
      virtual bool Initialize() = 0;
      virtual bool Shutdown() = 0;
      virtual void StepTime(double dt) = 0;
      virtual void AddObject(Node* obj, ObjectType type, PhysicsMaterial mat) = 0;
      virtual void RemoveObject(Node* obj) = 0;
      virtual void ApplyForce(Node* obj, glm::vec3 force) = 0;
      virtual void ApplyTorque(Node* obj, glm::vec3 torque) = 0;
    };
  }
}
#endif // PHYSICSENGINE_H
