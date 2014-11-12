#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SimpleDemo.h"


using namespace Projects::Demo::SimpleDemo;
using namespace RealisticEngine::Renderer;
using namespace RealisticEngine::Core;
using namespace RealisticEngine::Scene;



void SimpleDemoState::Initialize()
{
  SDLState::Initialize(); // initialize window

  mShader1.LoadShader("shader.vert", "shader.frag"); // load shader
  if(mShader1.Compile() == false) // if failed to build
  {
    std::cout << mShader1.GetVertexShaderLog() << "\n\n";
    std::cout << mShader1.GetFragmentShaderLog() << "\n\n";
    std::cout.flush();
  }
  if(mShader1.Link() == false)
  {
    std::cout << "failed to link shader\n";
    std::cout.flush();
  }


  mShader1.MakeActive(); // make active

  mGPURenderer.SetActiveShader(&mShader1);

  // initialze shader values
  mProjMatTransform = glm::perspective(45.0f, 1024.0f/768.0f, 0.1f, 10000.0f);
  mViewMatTransform = glm::mat4(1);
  mRootNodeTransform = glm::mat4(1);

  mViewMatTransform = glm::translate(mViewMatTransform, glm::vec3(0.0,-300,-1000.0));
  mRootNodeTransform = glm::rotate(mRootNodeTransform, -3.141592654f/2.0f, glm::vec3(1.0f,0.0f,0.0f));

  // setup uniform variables
  mProjMat.Setup("projMat", UniformVariable::MAT4F, 1);
  mViewMat.Setup("viewMat", UniformVariable::MAT4F, 1);

  // setup uniform variables to point to current active shader
  mProjMat.BindActiveShader(&mGPURenderer);
  mViewMat.BindActiveShader(&mGPURenderer);

  // setup uniform variables to point to defined variables
  mProjMat.BindData(glm::value_ptr(mProjMatTransform));
  mViewMat.BindData(glm::value_ptr(mViewMatTransform));


  // setup node transforms
  mRootNode.SetLocalTransform(mRootNodeTransform);

  // add draw interfaces to the nodes
  // must add uniform variables in order for them to be updated in the shader
  mRootNode.AddAsset(&mProjMat);
  mRootNode.AddAsset(&mViewMat);


  Node* node = new Node();
  mModelLoader.Setup(&mGPURenderer);
//  mModelLoader.Load("./Beautiful Girl", "Beautiful Girl.obj", node);
  mModelLoader.Load("./BumbleBee/", "RB-BumbleBee.obj", node);

  mRootNode.AddChild(node);



  glClearColor(0,0,0,1);
  glEnable(GL_DEPTH_TEST);
}

void SimpleDemoState::Update()
{
  SDLState::Update();

  // update scene
  mRootNode.Update();

  // set new rootNode transform
  mRootNodeTransform = glm::rotate(mRootNodeTransform, 0.05f, glm::vec3(0.0f,0.0f,1.0f));
  mRootNode.SetLocalTransform(mRootNodeTransform);



  if(mEngine->GetTime() > 100.0)
  {
    mEngine->Stop();
  }
}

void SimpleDemoState::Render(double delta)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // render scene
  mRootNode.Draw(delta);

  SDL_RenderPresent(SDLState::mRenderer);
}

void SimpleDemoState::ProcessEvent(SDL_Event env)
{
  switch(env.type)
  {
  case SDL_QUIT:
    mEngine->Stop();
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
