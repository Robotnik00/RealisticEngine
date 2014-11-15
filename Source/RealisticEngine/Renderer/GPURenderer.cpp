
#include <iostream>

#include <GL/gl.h>

#include "RealisticEngine/Renderer/GPURenderer.h"
#include "RealisticEngine/Scene/Node.h"

using namespace RealisticEngine::Renderer;
using namespace RealisticEngine::Scene;


void GPURenderer::Initialize()
{
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


  SetActiveShader(&mShader1);

}

void GPURenderer::RenderScene(Node* scene, double delta)
{
  if(scene != NULL)
  {
    SetActiveShader(&mShader1);

    scene->Draw(delta);
  }
}
