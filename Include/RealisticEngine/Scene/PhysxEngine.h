#ifndef PHYSXENGINE_H
#define PHYSXENGINE_H

#include <map>

#define NDEBUG
#include <PxPhysicsAPI.h>

#include "RealisticEngine/Scene/PhysicsEngine.h"

namespace RealisticEngine
{
  namespace Scene
  {

    class PhysxEngine : public PhysicsEngine
    {
    public:
      virtual void Setup(double gravity);
      virtual bool Initialize();
      virtual bool Shutdown();
      virtual void StepTime(double dt);
      virtual void AddObject(Node* node, ObjectType type, PhysicsMaterial mat);
      virtual void RemoveObject(Node* node);
      virtual void ApplyForce(Node* node, glm::vec3 force);
      virtual void ApplyTorque(Node* node, glm::vec3 torque);
    protected:

      void AddGeometryRecursive(Node* rootnode, Node* subnode, physx::PxRigidActor* actor, PhysicsMaterial* mat, ObjectType type);

      static physx::PxDefaultErrorCallback mDefaultErrorCB;
      static physx::PxDefaultAllocator mDefaultAllocator;

      physx::PxProfileZoneManager* mProfileZoneManager;
      physx::PxFoundation* mFoundation;
      physx::PxPhysics* mPhysics;
      physx::PxCooking* mCooking;
      physx::PxCpuDispatcher* mCpuDispatcher;
      physx::PxParticleFluid* mParticleFluid;

      physx::PxScene* mScene;

      double mGravity;

      long mLastID = 0;

      std::map<long, Node*> mActors;
      std::map<Node*, physx::PxRigidDynamic*> mDynamicActors;
    };
  }
}

#endif // PHYSXENGINE_H
