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
  std::cout << randFloat() << std::endl;
  Node* node =  RealisticEngine::Scene::CreateCube(randFloat()+.3, randFloat()+.3, randFloat()+.3, renderer);
  Material* mat1 = new Material();
  mat1->Setup("material", renderer);
  mat1->SetDiffuseReflectance(glm::vec3(randFloat(), randFloat(), randFloat()));
  node->AddAsset(mat1);
  node->Translate(glm::vec3(randFloat()*2-1.0,randFloat()*5+10,randFloat()*2-1.0));

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

  mPhysx.Setup(-9.81);
  mPhysx.Initialize();

  mGPURenderer.Initialize();


  Light* light1 = new Light();
  light1->Setup("light", &mGPURenderer);
  light1->SetColor(glm::vec3(1,1,1));
  light1->SetPosition(glm::vec3(10,10,10));
  light1->SetQuadraticAttenuation(0.01);
  mGPURenderer.AddLight(light1);

  Light* light2 = new Light();
  light2->Setup("light", &mGPURenderer);
  light2->SetColor(glm::vec3(1,1,1));
  light2->SetPosition(glm::vec3(-10,10,-10));
  light2->SetQuadraticAttenuation(0.01);
  mGPURenderer.AddLight(light2);

//  Light* light3 = new Light();
//  light3->Setup("light", &mGPURenderer);
//  light3->SetColor(glm::vec3(0,1,0));
//  light3->SetPosition(glm::vec3(-400,400,400));
//  light3->SetQuadraticAttenuation(0.00001);
//  mGPURenderer.AddLight(light3);

//  Light* light4 = new Light();
//  light4->Setup("light", &mGPURenderer);
//  light4->SetColor(glm::vec3(0,0,1));
//  light4->SetPosition(glm::vec3(-400,400,-400));
//  light4->SetQuadraticAttenuation(0.00001);
//  mGPURenderer.AddLight(light4);

//  Light* light5 = new Light();
//  light5->Setup("light", &mGPURenderer);
//  light5->SetColor(glm::vec3(1,0,1));
//  light5->SetPosition(glm::vec3(400,400,-400));
//  light5->SetQuadraticAttenuation(0.00001);
//  mGPURenderer.AddLight(light5);


  mCamera.Setup("cameraPosition", "viewMat", "projMat", &mGPURenderer);
  mCamera.SetSceneNode(&mRootNode);
  mCamera.PerspectiveMatrix(45.0,1024.0,768.0, .1,100.0);
  mCamera.SetPosition(0,10,10);

  mnode1 = RealisticEngine::Scene::CreateCube(10, 1, 1, &mGPURenderer);
  Material* mat1 = new Material();
  mat1->Setup("material", &mGPURenderer);
  mat1->SetShininess(100);
  mat1->SetDiffuseReflectance(glm::vec3(1.0,0.7,0.7));
  mnode1->AddAsset(mat1);
  glm::mat4 scalemat = glm::mat4(1);
  scalemat = glm::scale(scalemat, glm::vec3(2,.5,.5));
  mnode1->SetLocalTransform(scalemat);
  mnode1->Rotate(0, glm::vec3(0,1,0));
  mnode1->Translate(glm::vec3(0,15,0));
  Node* node = new Node();
  node->AddChild(mnode1);
//  mRootNode.AddChild(node);

  PhysicsMaterial mat;
  mat.mStaticFriction = .5;
  mat.mDynamicFriction = .5;
  mat.mRestitution = .6;
  mat.mDensity = 1.0;
//  mPhysx.AddObject(node, PhysxEngine::RIGID_DYNAMIC, mat);


  mnode2 = RealisticEngine::Scene::CreateCube(100, 1, 100, &mGPURenderer);
  Material* mat2 = new Material();
  mat2->Setup("material", &mGPURenderer);
  mat2->SetShininess(0);
  mnode2->AddAsset(mat2);
  mnode2->Translate(glm::vec3(0,0,0));
  mRootNode.AddChild(mnode2);
  mPhysx.AddObject(mnode2, PhysxEngine::RIGID_STATIC, mat);

//  Node* nodey = new Node();
//  Node* node3 = RealisticEngine::Scene::CreateCube(1, 1, 1, &mGPURenderer);
//  node3->AddAsset(mat1);
//  glm::mat4 scale = glm::mat4(1);
//  scale = glm::scale(scale, glm::vec3(2,2,2));
//  node3->SetLocalTransform(scale);
//  nodey->Translate(glm::vec3(2,10,0));
//  nodey->AddChild(node3);
//  mRootNode.AddChild(nodey);
//  mPhysx.AddObject(nodey, PhysxEngine::RIGID_DYNAMIC, mat);

//  Node* node4 = RealisticEngine::Scene::CreateCube(1, 1, 1, &mGPURenderer);
//  node4->AddAsset(mat1);
//  node4->Translate(glm::vec3(-2,-3,3));

//  mnode1->AddChild(node4);




  Node* node5 = new Node();
//  Node* node4 = new Node();

  mModelLoader.Setup(&mGPURenderer);
//  mModelLoader.Load("./Boy", "boy.obj", mnode1);
//  mModelLoader.Load("./Lincoln Navigator", "navigator.obj", node5);
//  mModelLoader.Load("./Beautiful Girl", "Beautiful Girl.obj", node5);
//  mModelLoader.Load("./N916MU/Formats", "N916MU.3ds", mnode1);
//  mModelLoader.Load("./BumbleBee/", "RB-BumbleBee.obj", node3);
  mModelLoader.Load("./BumbleBee/", "RB-BumbleBee.obj", node5);

//  mRootNode.AddChild(node3);
//  node3->SetLocalTransform(scale);
//  node3->Translate(glm::vec3(2000,2000,0));
//  mPhysx.AddObject(node3, PhysicsEngine::RIGID_DYNAMIC, mat);

  Node* node6 = new Node();
  node6->AddChild(node5);
  mRootNode.AddChild(node6);
  glm::mat4 bumblescale = glm::mat4(1);
  bumblescale = glm::scale(bumblescale, glm::vec3(0.01,0.01,0.01));
  node5->SetLocalTransform(bumblescale);

  node5->Rotate(-90, glm::vec3(1,0,0));

  node6->Translate(glm::vec3(0,10,0));
  mPhysx.AddObject(node6, PhysxEngine::RIGID_DYNAMIC, mat);



  mRootNode.AddAction(&mCamera);
  mRootNode.AddAsset(&mCamera);
  mCamera.SetSceneNode(&mRootNode);




  for(int i = 0; i < 256; i++)
    mKeysPressed[i] = false;

  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
}

void SimpleDemoState::Update()
{
  SDLState::Update();

  std::string title = std::to_string(mEngine->GetFrameRate());
  SetWindowTitle(title.data());

  // update scene
  mRootNode.Update();

  float speed = .1f;
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


  if(mEngine->GetTime() - lastTime > 0.25)
  {
    mRootNode.AddChild(CreateRandomObject(&mGPURenderer, &mPhysx));
    lastTime = mEngine->GetTime();
  }




  mPhysx.StepTime(1.0/30.0);
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
