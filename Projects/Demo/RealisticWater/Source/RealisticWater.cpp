#include <GL/gl.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include <RealisticEngine/Scene/Primitives.h>
#include <RealisticEngine/Utility/Utility.h>

#include "RealisticWater.h"

using namespace Projects::Demo::RealisticWater;
using namespace RealisticEngine::Core;
using namespace RealisticEngine::Scene;
using namespace RealisticEngine::Renderer;
float randFloat()
{
  return ((float)rand())/((float)RAND_MAX);
}


Node* CreateBox(glm::vec3 pos, glm::vec3 size, GPURenderer* renderer, PhysicsEngine* physx)
{
  Node* node =  RealisticEngine::Scene::CreateCube(size.x, size.y, size.z, renderer);
  Material* mat1 = new Material();
  mat1->Setup("material", renderer);
  mat1->SetDiffuseReflectance(glm::vec3(randFloat(), randFloat(), randFloat()));
//  mat1->SetDiffuseReflectance(glm::vec3(0,.5,1));
  node->AddAsset(mat1);
  node->Translate(pos);

  PhysicsMaterial mat;
  mat.mStaticFriction = .5;
  mat.mDynamicFriction = .5;
  mat.mRestitution = .1;
  mat.mDensity = 0.1;
  physx->AddObject(node, PhysicsEngine::RIGID_DYNAMIC, mat);
  return node;
}

void RealisticWaterState::Initialize()
{
  SDLState::Initialize(); // initialize window

  mPhysx.Setup(-9.81);
  mPhysx.Initialize();
  mGPURenderer.Initialize();
  mModelLoader.Setup(&mGPURenderer);



  // camera
  mCamera.Setup("cameraPosition", "viewMat", "projMat", &mGPURenderer);
  mCamera.SetSceneNode(&mRootNode);
  mCamera.PerspectiveMatrix(45.0,1024.0,768.0, .1,100.0);
  mCamera.SetPosition(0,2.5,5);
  mRootNode.AddAction(&mCamera);
  mRootNode.AddAsset(&mCamera);
  mCamera.SetSceneNode(&mRootNode);

  mPS.Setup(&mCamera, &mGPURenderer);
  mPS.Initialize();

  mGPURenderer.SetParticleSystem(&mPS);


  // light1
  Light* light1 = new Light();
  light1->Setup("light", &mGPURenderer);
  light1->SetColor(glm::vec3(1,1,1));
  light1->SetPosition(glm::vec3(25,50,35));
  light1->SetQuadraticAttenuation(0.0001);
  mGPURenderer.AddLight(light1);
  // light1
  Light* light2 = new Light();
  light2->Setup("light", &mGPURenderer);
  light2->SetColor(glm::vec3(1,1,1));
  light2->SetPosition(glm::vec3(-35,50,50));
  light2->SetQuadraticAttenuation(0.0001);
  mGPURenderer.AddLight(light2);

  PhysicsMaterial mat;
  mat.mDensity = .5;
  mat.mDynamicFriction = 0.5;
  mat.mStaticFriction = 0.5;
  mat.mRestitution = 0.5;


  Node* baseObjSubnode = new Node();
  mModelLoader.Load("./", "boxwithhole.obj", baseObjSubnode);
  Node* baseObjRootnode = new Node();
  baseObjRootnode->AddChild(baseObjSubnode);
  glm::mat4 scale = glm::mat4(1.0);
  scale = glm::scale(scale, glm::vec3(1.5,2,1.5));
  baseObjSubnode->SetLocalTransform(scale);

//  mRootNode.AddChild(baseObjRootnode);
  mPhysx.AddObject(baseObjRootnode, PhysicsEngine::RIGID_STATIC, mat);


  std::vector<uint32_t> ind;
  uint32_t index = 0;

  /*
   * set positions of Particles
   * added indices(needed for physx system
   */

  for(GLfloat k = 0; k < 40; k+= .01) // a bit of a hack works for now..
  {
    for(GLfloat i = 0; i < .1; i+= .05)
    {
      for(GLfloat j = 0; j < .1; j+= .05)
      {
        //      glm::vec3 tmp = glm::vec3(i-10, i*j/10, j-10);

        mPositions[3*index+0] = i-.05;
        mPositions[3*index+1] = k/100;
        mPositions[3*index+2] = j-.05;

        //      pos.push_back(tmp);
        ind.push_back(index++);
      }
    }
  }

  std::cout << "number of particles " << ind.size() << std::endl;

  mNumParticles = index;
  if(mPhysx.CreateFluid(ind.size(), ind.data(), mPositions) == false)
  {
    std::cout << "success creating fluid\n";
  }


  // initial key states
  for(int i = 0; i < 256; i++)
    mKeysPressed[i] = false;
  std::cout.flush();
  glClearColor(0.7,0.7,0.7,1);
}

void RealisticWaterState::Update()
{
  SDLState::Update();


  mPhysx.RealFluidParticlePositions(mPositions);
  mPS.SetParticles(mPositions, mNumParticles);
  // update scene
  mRootNode.Update();

  char buffer[128];
  sprintf(buffer, "%f\n", mEngine->GetFrameRate());
  SetWindowTitle(buffer);

  float speed = 0.04f;
  if(mKeysPressed['w'])
  {
    glm::vec3 forward = glm::vec3(mCamera.GetCamZ());
    forward *= -speed;
    mCamera.Translate(forward);
  }
  if(mKeysPressed['s'])
  {
    glm::vec3 forward = glm::vec3(mCamera.GetCamZ());
    forward *= speed;
    mCamera.Translate(forward);
  }
  if(mKeysPressed['a'])
  {
    glm::vec3 side = glm::vec3(mCamera.GetCamX());
    side *= -speed;
    mCamera.Translate(side);
  }
  if(mKeysPressed['d'])
  {
    glm::vec3 side = glm::vec3(mCamera.GetCamX());
    side *= speed;
    mCamera.Translate(side);
  }
  if(mKeysPressed['z'])
  {
    glm::vec3 up = glm::vec3(mCamera.GetCamY());
    up *= speed;
    mCamera.Translate(up);
  }
  if(mKeysPressed['x'])
  {
    glm::vec3 down = glm::vec3(mCamera.GetCamY());
    down *= -speed;
    mCamera.Translate(down);
  }
  if(mKeysPressed[80] == true) // left
  {
    mCamera.Rotate(5.0, glm::vec3(mCamera.GetCamY()));
  }
  if(mKeysPressed[82] == true) // up
  {
    mCamera.Rotate(5.0, glm::vec3(mCamera.GetCamX()));
  }
  if(mKeysPressed[79] == true) // right
  {
    mCamera.Rotate(-5.0, glm::vec3(mCamera.GetCamY()));
  }
  if(mKeysPressed[81] == true) // down
  {
    mCamera.Rotate(-5.0, glm::vec3(mCamera.GetCamX()));
  }
  if(mKeysPressed['q'] == true) // z axis rotate
  {
    mCamera.Rotate(5.0, glm::vec3(mCamera.GetCamZ()));
  }
  if(mKeysPressed['e'] == true) // z axis rotate
  {
    mCamera.Rotate(-5.0, glm::vec3(mCamera.GetCamZ()));
  }

  if(mKeysPressed['p']) // change engine tick frequency
  {
    mEngine->SetUpdateFrequency(mEngine->GetUpdateFrequency() + 1);
  }
  else if(mKeysPressed['n'])
  {
    mEngine->SetUpdateFrequency(mEngine->GetUpdateFrequency() - 1);
  }

  if(mKeysPressed['g'] && mEngine->GetTime() - mLastTime > 0.5)
  {

    Node* box = CreateBox(glm::vec3(randFloat() * 1.5 - 0.75,7,randFloat()*1.5 -0.75), glm::vec3(0.5,0.5,0.5), &mGPURenderer, &mPhysx);
    mRootNode.AddChild(box);
    mLastTime = mEngine->GetTime();
  }


  if(glm::length(mMouseVec) > 0.1)
  {
    glm::vec3 mousevec = glm::normalize(glm::vec3(mCamera.GetMatrix() * mMouseVec));
    mousevec = glm::cross(glm::vec3(mCamera.GetCamZ()), mousevec);
    mCamera.Rotate(4.0 * glm::length(mMouseVec), mousevec);
  }


  // constant stepping
  // makes it so when engine ticks reduced physics is same accuracy but runs in slow motion
  // ideally this should change with engine tick frequency but it may get rly inaccurate
  // at slow tick rate
  mPhysx.StepTime(1/60.0);
}

void RealisticWaterState::Render(double delta)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // render scene starting from the root node and traversing down
  mGPURenderer.RenderScene(&mRootNode, delta);

  SDL_RenderPresent(SDLState::mRenderer);
}

void RealisticWaterState::ProcessEvent(SDL_Event env)
{
  // process SDL event
  switch(env.type)
  {
  case SDL_QUIT:
    mEngine->Stop();
    break;
  case SDL_MOUSEMOTION:
    mMouseVec = glm::vec4(-((float)env.motion.x - 511.0f)/511.0f, ((float)env.motion.y - 383.0f)/383.0f, 0.0, 0.0);
    break;
  case SDL_KEYDOWN:
    mKeysPressed[env.key.keysym.sym%256] = true; // hack
    break;
  case SDL_KEYUP:
    mKeysPressed[env.key.keysym.sym%256] = false;
    break;

  default:
    break;
  }
}


int main(int argc, char** argv)
{
  Engine engine;
  RealisticWaterState demoState(&engine);
  demoState.Initialize();
  engine.SetState(&demoState);
  engine.Start();
  return 0;
}
