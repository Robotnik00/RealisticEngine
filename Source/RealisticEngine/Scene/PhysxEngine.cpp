#include <iostream>

#include "RealisticEngine/Scene/PhysxEngine.h"
#include "RealisticEngine/Scene/Node.h"

#include <glm/gtx/quaternion.hpp>

using namespace physx;
using namespace RealisticEngine::Scene;


PxDefaultErrorCallback RealisticEngine::Scene::PhysxEngine::mDefaultErrorCB;
PxDefaultAllocator RealisticEngine::Scene::PhysxEngine::mDefaultAllocator;

void PhysxEngine::Setup(double gravity)
{
  mGravity = gravity;
}

bool PhysxEngine::Initialize()
{
  std::cout << "initializing physx...\n";
  mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocator, mDefaultErrorCB);
  if(!mFoundation)
  {
    std::cout << "could not create physx foundation\n";
    return false;
  }

  mProfileZoneManager = &PxProfileZoneManager::createProfileZoneManager(mFoundation);
  if(!mProfileZoneManager)
  {
    std::cout << "could not get create profile zone manager\n";
    return false;
  }

  mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), true, mProfileZoneManager );
  if(!mPhysics)
  {
    std::cout << "could not initialize physics\n";
    return false;
  }

  mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams(mPhysics->getTolerancesScale()));
  if(!mCooking)
  {
    std::cout << "could not initialize cooking\n";
    return false;
  }

  physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
  sceneDesc.gravity = physx::PxVec3(0.0f, mGravity, 0.0f);

  mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(4);
  if(!mCpuDispatcher)
  {
    std::cout << "error could not create cpu dispatcher\n";
    return false;
  }

  sceneDesc.cpuDispatcher = mCpuDispatcher;
  sceneDesc.filterShader  = physx::PxDefaultSimulationFilterShader;

  mScene = mPhysics->createScene(sceneDesc);
  if(!mScene)
  {
    std::cout << "could not create scene\n";
    return false;
  }








  std::cout << "physx initialized!\n";
  std::cout.flush();
  return true;
}

bool PhysxEngine::Shutdown()
{
  mPhysics->release();
  mFoundation->release();
  return true;
}

void PhysxEngine::StepTime(double dt)
{

  physx::PxActorTypeFlags desiredTypes = physx::PxActorTypeFlag::eRIGID_STATIC | physx::PxActorTypeFlag::eRIGID_DYNAMIC;
  physx::PxU32 count = mScene->getNbActors(desiredTypes);
  physx::PxActor** buffer = new physx::PxActor*[count];
  mScene->getActors(desiredTypes, buffer, count);
  for(physx::PxU32 i = 0; i < count; i++)
  {
      if(buffer[i]->getOwnerClient() == physx::PX_DEFAULT_CLIENT)
      {
          physx::PxRigidActor* actor = buffer[i]->isRigidDynamic();
          if(actor != NULL)
          {
            physx::PxMat44 mat(actor->getGlobalPose());
            mActors[(long)actor->userData]->SetLocalTransform(*((glm::mat4*)&mat));
          }
      }
  }


  mScene->simulate(dt);
  mScene->fetchResults(true);
}

glm::mat4 GetCoordinateSpace(Node* rootnode, Node* subnode)
{
  if(subnode == rootnode)
  {
    return glm::mat4(1);
  }

  return GetCoordinateSpace(rootnode, subnode->GetParent()) * subnode->GetLocalTransform();
}

void PhysxEngine::AddGeometryRecursive(Node* rootnode, Node *subnode, PxRigidActor *actor, PhysicsMaterial* mat, ObjectType type)
{
  glm::mat4 relativepos = GetCoordinateSpace(rootnode, subnode);

  std::vector<PxVec3> verts;
  for(int i = 0; i < subnode->GetGeometry().mVertices.size(); i++)
  {
    glm::vec3 tmp = subnode->GetGeometry().mVertices[i];
    verts.push_back(PxVec3(tmp.x, tmp.y, tmp.z));
  }
  PxBoundedData indices;
  indices.stride = sizeof(GLint);
  if(verts.size() > 0)
  {
    if(type == RIGID_DYNAMIC)
    {
      PxConvexMeshDesc convexDesc;
      convexDesc.points.count     = verts.size();
      convexDesc.points.stride    = sizeof(physx::PxVec3);
      convexDesc.points.data      = verts.data();
      convexDesc.flags            = PxConvexFlag::eCOMPUTE_CONVEX | physx::PxConvexFlag::eINFLATE_CONVEX;
      convexDesc.vertexLimit      = 256;
      convexDesc.triangles.data   = subnode->GetGeometry().mIndices.data();
      convexDesc.triangles.stride = 3 * sizeof(PxU32);
      convexDesc.triangles.count  = subnode->GetGeometry().mIndices.size()/3;

      std::cout << "cooking... " << verts.size() << std::endl;
      std::cout.flush();
      PxDefaultMemoryOutputStream buf;
      if(!mCooking->cookConvexMesh(convexDesc, buf))
      {
        std::cout << "failed cooking convexmesh\n";
      }
      else
      {
        PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
        PxConvexMesh* convexMesh = mPhysics->createConvexMesh(input);

        glm::vec3 xaxis = glm::vec3(relativepos[0]);
        glm::vec3 yaxis = glm::vec3(relativepos[1]);
        glm::vec3 zaxis = glm::vec3(relativepos[2]);

        double scalex = glm::length(xaxis);
        double scaley = glm::length(yaxis);
        double scalez = glm::length(zaxis);

        glm::mat4 normalizedmat = glm::mat4(glm::normalize(relativepos[0]),
            glm::normalize(relativepos[1]),
            glm::normalize(relativepos[2]),
            relativepos[3]);



        PxMeshScale scale(PxVec3(scalex,scaley,scalez), PxQuat(1.0));
        PxConvexMeshGeometry geoconvex = PxConvexMeshGeometry(convexMesh, scale);

        PxMaterial* material;

        material = mPhysics->createMaterial(mat->mStaticFriction, mat->mDynamicFriction, mat->mRestitution);
        if(!material)
        {
          std::cout << "could not create material\n";
        }

        PxShape* shape = mPhysics->createShape(geoconvex, *material);
        shape->setLocalPose(PxTransform(*((physx::PxMat44*)&normalizedmat)));
        actor->attachShape(*shape);
      }
    }
    else if(type == RIGID_STATIC)
    {
      PxTriangleMeshDesc meshDesc;
      meshDesc.points.count           = verts.size();
      meshDesc.points.stride          = sizeof(PxVec3);
      meshDesc.points.data            = verts.data();
      std::vector<PxU32> indices;
      for(int i = 0; i < subnode->GetGeometry().mIndices.size(); i++)
      {
        indices.push_back(subnode->GetGeometry().mIndices[i]);
      }
      meshDesc.triangles.count        = indices.size()/3;
      meshDesc.triangles.stride       = 3*sizeof(PxU32);
      meshDesc.triangles.data         = indices.data();

      std::cout << "cooking... " << verts.size() << std::endl;
      std::cout.flush();
      PxDefaultMemoryOutputStream buf;
      if(!mCooking->cookTriangleMesh(meshDesc, buf))
      {
        std::cout << "failed cooking convexmesh\n";
      }
      else
      {
        PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
        PxTriangleMesh* convexMesh = mPhysics->createTriangleMesh(input);

        glm::vec3 xaxis = glm::vec3(relativepos[0]);
        glm::vec3 yaxis = glm::vec3(relativepos[1]);
        glm::vec3 zaxis = glm::vec3(relativepos[2]);

        double scalex = glm::length(xaxis);
        double scaley = glm::length(yaxis);
        double scalez = glm::length(zaxis);

        glm::mat4 normalizedmat = glm::mat4(glm::normalize(relativepos[0]),
            glm::normalize(relativepos[1]),
            glm::normalize(relativepos[2]),
            relativepos[3]);



        PxMeshScale scale(PxVec3(scalex,scaley,scalez), PxQuat(1.0));
        PxTriangleMeshGeometry geoconvex = PxTriangleMeshGeometry(convexMesh, scale);

        PxMaterial* material;

        material = mPhysics->createMaterial(mat->mStaticFriction, mat->mDynamicFriction, mat->mRestitution);
        if(!material)
        {
          std::cout << "could not create material\n";
        }

        PxShape* shape = mPhysics->createShape(geoconvex, *material);
        shape->setLocalPose(PxTransform(*((physx::PxMat44*)&normalizedmat)));
        actor->attachShape(*shape);
      }
    }


  }
  for(int i = 0; i < subnode->GetChildren().size(); i++)
  {
    AddGeometryRecursive(rootnode, subnode->GetChildren()[i], actor, mat, type);
  }
}

void PhysxEngine::AddObject(Node *node, ObjectType type, PhysicsMaterial mat)
{

  PxMaterial* material;

  material = mPhysics->createMaterial(mat.mStaticFriction, mat.mDynamicFriction, mat.mRestitution);
  if(!material)
  {
    std::cout << "could not create material\n";
  }


  PxRigidActor* actor;

  glm::mat4 transform = node->GetLocalTransform();

  switch(type)
  {
  case RIGID_DYNAMIC:
    actor = mPhysics->createRigidDynamic(PxTransform(*((physx::PxMat44*)&transform)));
    AddGeometryRecursive(node, node, actor, &mat, RIGID_DYNAMIC);
    physx::PxRigidBodyExt::updateMassAndInertia(*(physx::PxRigidDynamic*)actor, mat.mDensity);
    mDynamicActors[node] = (PxRigidDynamic*)actor;
    break;
  case RIGID_STATIC:
    actor = mPhysics->createRigidStatic(PxTransform(*((physx::PxMat44*)&transform)));
    AddGeometryRecursive(node, node, actor, &mat, RIGID_STATIC);
    break;
  }

  actor->userData = (void*)mLastID;
  mScene->addActor(*actor);
  mActors[mLastID] = node;
  mLastID++;
}

void PhysxEngine::RemoveObject(Node *node)
{
  /*
   *  to do
   */
}

bool PhysxEngine::CreateFluid(uint32_t numParticles, uint32_t *indices, float *startingpositions)
{
  mParticleFluid = mPhysics->createParticleFluid(1000000, false);
  mMaxParticles = numParticles;

  mParticleFluid->setRestParticleDistance(.15);

  if(mParticleFluid != NULL)
    mScene->addActor(*mParticleFluid);


  PxParticleCreationData particleCreationData;
  particleCreationData.numParticles = numParticles;
  particleCreationData.positionBuffer = PxStrideIterator<const PxVec3>((PxVec3*)startingpositions);
  particleCreationData.indexBuffer = PxStrideIterator<const PxU32>(indices);

  mParticleFluid->setViscosity(.8);

//  mParticleFluid->setStiffness(1);
//  mParticleFluid->setDamping(0.7);
  bool success = mParticleFluid->createParticles(particleCreationData);

  return success;
}

bool PhysxEngine::RealFluidParticlePositions(float* data)
{
  PxParticleReadData* rd = mParticleFluid->lockParticleReadData();


  if(rd)
  {
    PxStrideIterator<const PxParticleFlags> flagsIt(rd->flagsBuffer);
    PxStrideIterator<const PxVec3> positionIt(rd->positionBuffer);
    int index = 0;
    for (unsigned i = 0; i < rd->validParticleRange; ++i, ++flagsIt, ++positionIt)
    {
            if (*flagsIt & PxParticleFlag::eVALID)
            {
              index++;

                    // access particle position
                    const PxVec3& position = *positionIt;
                    data[i*3 + 0] = position.x;
                    data[i*3 + 1] = position.y;
                    data[i*3 + 2] = position.z;
            }
    }
    // return ownership of the buffers back to the SDK
    rd->unlock();
    return true;
  }


  return false;


}

void PhysxEngine::ApplyForce(Node *node, glm::vec3 force)
{
  /*
   * to do
   */


}

void PhysxEngine::ApplyTorque(Node *node, glm::vec3 torque)
{
  /*
   * to do
   */
}
