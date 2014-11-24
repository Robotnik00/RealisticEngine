#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <RealisticEngine/Scene/Camera.h>
#include <RealisticEngine/Scene/Primitives.h>


#include "SimpleDemo.h"


using namespace Projects::Demo::SimpleDemo;
using namespace RealisticEngine::Core;
using namespace RealisticEngine::Scene;
using namespace RealisticEngine::Renderer;

float randFloat()
{
  return ((float)rand())/((float)RAND_MAX);
}

Node* CreateRandomObject(GPURenderer* renderer, PhysicsEngine* physx)
{
  Node* node =  RealisticEngine::Scene::CreateCube(randFloat()+.5, randFloat()+.5, randFloat()+.5, renderer);
  Material* mat1 = new Material();
  mat1->Setup("material", renderer);
  mat1->SetDiffuseReflectance(glm::vec3(randFloat(), randFloat(), randFloat()));
  node->AddAsset(mat1);
  node->Translate(glm::vec3(randFloat()*2-1.0,randFloat()*2+15, randFloat()*2-1.0));

  PhysicsMaterial mat;
  mat.mStaticFriction = .5;
  mat.mDynamicFriction = .5;
  mat.mRestitution = .1;
  mat.mDensity = 1.0;
  physx->AddObject(node, PhysicsEngine::RIGID_DYNAMIC, mat);
  return node;
}

Node* CreateBox(glm::vec3 pos, glm::vec3 size, GPURenderer* renderer, PhysicsEngine* physx)
{
  Node* node =  RealisticEngine::Scene::CreateCube(size.x, size.y, size.z, renderer);
  Material* mat1 = new Material();
  mat1->Setup("material", renderer);
//  mat1->SetDiffuseReflectance(glm::vec3(randFloat(), randFloat(), randFloat()));
  mat1->SetDiffuseReflectance(glm::vec3(0,.5,1));
  node->AddAsset(mat1);
  node->Translate(pos);

  PhysicsMaterial mat;
  mat.mStaticFriction = .5;
  mat.mDynamicFriction = .5;
  mat.mRestitution = .1;
  mat.mDensity = 1.0;
  physx->AddObject(node, PhysicsEngine::RIGID_DYNAMIC, mat);
  return node;
}


void SimpleDemoState::Initialize()
{
  SDLState::Initialize(); // initialize window

  // initialize things
  mPhysx.Setup(-9.81);
  mPhysx.Initialize();
  mGPURenderer.Initialize();
  mModelLoader.Setup(&mGPURenderer);

  // camera
  mCamera.Setup("cameraPosition", "viewMat", "projMat", &mGPURenderer);
  mCamera.SetSceneNode(&mRootNode);
  mCamera.PerspectiveMatrix(45.0,1024.0,768.0, 1,1000.0);
  mCamera.SetPosition(0,15,50);
  mRootNode.AddAction(&mCamera);
  mRootNode.AddAsset(&mCamera);
  mCamera.SetSceneNode(&mRootNode);

  // light1
  Light* light1 = new Light();
  light1->Setup("light", &mGPURenderer);
  light1->SetColor(glm::vec3(1,1,1));
  light1->SetPosition(glm::vec3(25,50,25));
  light1->SetQuadraticAttenuation(0.0001);
  mGPURenderer.AddLight(light1);

  // physics material
  PhysicsMaterial pxmat;
  pxmat.mDensity = 0.5;
  pxmat.mDynamicFriction = 0.7;
  pxmat.mStaticFriction = 0.5;
  pxmat.mRestitution = 0.6;

  // create static base
  Node* subnode1 = new Node();
  mModelLoader.Load("./", "boxwithhole.obj", subnode1);
  Node* rootnode1 = new Node();
  rootnode1->AddChild(subnode1);
  mRootNode.AddChild(rootnode1);
  glm::mat4 scalemat = glm::mat4(1);
  scalemat = glm::scale(scalemat, glm::vec3(15,4,15));
  subnode1->SetLocalTransform(scalemat);
  rootnode1->Translate(glm::vec3(0,0,0));
  mPhysx.AddObject(rootnode1, PhysicsEngine::RIGID_STATIC, pxmat);

  // create NxNxN boxes
  int n = 10;
  float scale = 1;
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < n; j++)
    {
      for(int k = 0; k < n; k++)
      {
        glm::vec3 pos = glm::vec3(scale * (i - n/2),scale * j + 20, scale * (k-n/2));
        glm::vec3 size = glm::vec3(scale*.9,scale*.9,scale*.9);
        Node* node = CreateBox(pos, size, &mGPURenderer, &mPhysx);

        mRootNode.AddChild(node);

      }
    }
  }


  // initial key states
  for(int i = 0; i < 256; i++)
    mKeysPressed[i] = false;

  mEngine->SetUpdateFrequency(60);
}

void SimpleDemoState::Update()
{
  SDLState::Update();

  char buffer[128];
  sprintf(buffer, "U:%1.4f D:%3.2f A:%.4f", 1.0/mEngine->GetUpdateFrequency(), mEngine->GetFrameRate(), 1.0/60.0);
  SetWindowTitle(buffer);

  // update scene
  mRootNode.Update();

  float speed = 0.3f;
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
  if(mKeysPressed['q'] == true)
  {
    mCamera.Rotate(5.0, glm::vec3(mCamera.GetCamZ()));
  }
  if(mKeysPressed['e'] == true)
  {
    mCamera.Rotate(-5.0, glm::vec3(mCamera.GetCamZ()));
  }

  if(mKeysPressed['p'])
  {
    mEngine->SetUpdateFrequency(mEngine->GetUpdateFrequency() + 1);
  }
  else if(mKeysPressed['n'])
  {
    mEngine->SetUpdateFrequency(mEngine->GetUpdateFrequency() - 1);
  }


  if(glm::length(mMouseVec) > 0.1)
  {
    glm::vec3 mousevec = glm::normalize(glm::vec3(mCamera.GetMatrix() * mMouseVec));
    mousevec = glm::cross(glm::vec3(mCamera.GetCamZ()), mousevec);
    mCamera.Rotate(4.0 * glm::length(mMouseVec), mousevec);
  }


  if(mEngine->GetTime() - lastTime > 0.3)
  {
//    mRootNode.AddChild(CreateRandomObject(&mGPURenderer, &mPhysx));
    lastTime = mEngine->GetTime();
  }


  if(mBegin)
    mPhysx.StepTime(1.0/60.0);
}

void SimpleDemoState::Render(double delta)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  // render scene
  mGPURenderer.RenderScene(&mRootNode, delta);

  SDL_RenderPresent(SDLState::mRenderer);

}

void SimpleDemoState::ProcessEvent(SDL_Event env)
{
  switch(env.type)
  {
  case SDL_QUIT:
    mEngine->Stop();
    break;
  case SDL_MOUSEMOTION:
    mMouseVec = glm::vec4(-((float)env.motion.x - 511.0f)/511.0f, ((float)env.motion.y - 383.0f)/383.0f, 0.0, 0.0);
    break;
  case SDL_KEYDOWN:
    mKeysPressed[env.key.keysym.sym%256] = true;
    break;
  case SDL_KEYUP:
    mKeysPressed[env.key.keysym.sym%256] = false;
    if(env.key.keysym.sym == 'g')
    {
      mBegin = true;
    }
    break;

  default:
    break;
  }
}

int main(int argc, char** argv)
{
  std::cout << "SimpleDemo!\n";

  Engine engine;
  SimpleDemoState demoState(&engine);
  demoState.Initialize();
  engine.SetState(&demoState);
  engine.Start();

  return 0;
}
