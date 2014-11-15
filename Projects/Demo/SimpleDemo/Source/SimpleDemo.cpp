#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <RealisticEngine/Scene/Camera.h>


#include "SimpleDemo.h"


using namespace Projects::Demo::SimpleDemo;
using namespace RealisticEngine::Core;
using namespace RealisticEngine::Scene;
using namespace RealisticEngine::Renderer;

void SimpleDemoState::Initialize()
{
  SDLState::Initialize(); // initialize window

  mGPURenderer.Initialize();

  mCamera.Setup("cameraPosition", "viewMat", "projMat", &mGPURenderer);
  mCamera.SetSceneNode(&mRootNode);
  mCamera.PerspectiveMatrix(45.0,1024.0,768.0,10,4000.0);
  mCamera.SetPosition(0,200,1000);

  mnode1 = new Node();
//  mnode2 = new Node();
  mModelLoader.Setup(&mGPURenderer);
//  mModelLoader.Load("./Boy", "boy.obj", node1);
//  mModelLoader.Load("./Lincoln Navigator", "navigator.obj", node1);
//  mModelLoader.Load("./Beautiful Girl", "Beautiful Girl.obj", mnode2);
//  mModelLoader.Load("./N916MU/Formats", "N916MU.3ds", mnode1);
  mModelLoader.Load("./BumbleBee/", "RB-BumbleBee.obj", mnode1);
  mnode1->Rotate(-90, glm::vec3(1,0,0));
  mnode1->Rotate(-90, glm::vec3(0,0,1));

  mRootNode.AddChild(mnode1);

  mRootNode.AddAction(&mCamera);
  mRootNode.AddAsset(&mCamera);
  mCamera.SetSceneNode(&mRootNode);

  for(int i = 0; i < 256; i++)
    mKeysPressed[i] = false;

  mMouseVec = glm::vec4(0);
  glEnable(GL_DEPTH_TEST);
}

void SimpleDemoState::Update()
{
  SDLState::Update();

  std::string title = std::to_string(mEngine->GetFrameRate());
  SetWindowTitle(title.data());

  // update scene
  mRootNode.Update();

  float speed = 10.0f;
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

  if(glm::length(mMouseVec) > 0.1)
  {
    glm::vec3 mousevec = glm::normalize(glm::vec3(mCamera.GetMatrix() * mMouseVec));
    mousevec = glm::cross(glm::vec3(mCamera.GetCamZ()), mousevec);
    mCamera.Rotate(4.0 * glm::length(mMouseVec), mousevec);
  }

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
