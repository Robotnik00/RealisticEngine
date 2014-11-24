
#include <iostream>


#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RealisticEngine/Renderer/GPURenderer.h"
#include "RealisticEngine/Scene/Node.h"
#include "RealisticEngine/Renderer/Asset.h"

#include "RealisticEngine/Renderer/ParticleSystem.h"


using namespace RealisticEngine::Renderer;
using namespace RealisticEngine::Scene;


void GPURenderer::Initialize()
{
  mLightShader.LoadShader("lighting.vert", "lighting.frag"); // load shader
  if(mLightShader.Compile() == false) // if failed to build
  {
    std::cout << mLightShader.GetVertexShaderLog() << "\n\n";
    std::cout << mLightShader.GetFragmentShaderLog() << "\n\n";
    std::cout.flush();
  }
  if(mLightShader.Link() == false)
  {
    std::cout << "failed to link shader\n";
    std::cout.flush();
  }

  mDisplayShader.LoadShader("display.vert", "display.frag");
  if(mDisplayShader.Compile() == false)
  {
    std::cout << mDisplayShader.GetVertexShaderLog() << "\n\n";
    std::cout << mDisplayShader.GetFragmentShaderLog() << "\n\n";
  }
  if(mDisplayShader.Link() == false)
  {
    std::cout << "display shader not linked!\n";
  }
  if(mDepthCaptureShader.LoadShader("depth_capture.vert", "depth_capture.frag") == false)
  {
    std::cout << "could not find shader.\n";
  }
  if(mDepthCaptureShader.Compile() == false)
  {
    std::cout << mDepthCaptureShader.GetVertexShaderLog() << "\n\n";
    std::cout << mDepthCaptureShader.GetFragmentShaderLog() << "\n\n";
  }
  if(mDepthCaptureShader.Link() == false)
  {
    std::cout << "error\n";
  }
  std::cout.flush();


  SetActiveShader(&mLightShader);

  GLfloat displayPosition[] = {
    -1.0, -1.0, 0.0,
    1.0, -1.0, 0.0,
    1.0, 1.0, 0.0,
    -1.0, 1.0, 0.0
  };
  GLuint displayIndices[] = {
    0,1,2,
    0,2,3
  };

  mDisplayPositions.Setup(this, "position", displayPosition, GL_FLOAT, 12, 4, 3);
  mDisplayPositions.Load();

  mDisplayIndices.Setup(this, displayIndices, 6);
  mDisplayIndices.Load();

  mDisplayVBO.SetRenderer(this);
  mDisplayVBO.AddAtributeArray(&mDisplayPositions);
  mDisplayVBO.SetIndices(&mDisplayIndices);

  mFrameBufferObject1.Setup(1024, 768);
  mFrameBufferObject1.Initialize();

  mFrameBufferObject2.Setup(1024, 768);
  mFrameBufferObject2.Initialize();

  mLightMatrix.Setup("lightMat", UniformVariable::MAT4F, 1);
  mLightMatrix.SetRenderer(this);
  mTexUnit.Setup("depthMap", UniformVariable::INT, 1);
  mTexUnit.SetRenderer(this);


  mPS = NULL;

}

void GPURenderer::RenderScene(Node* scene, double delta)
{
  if(scene != NULL)
  {



    mFrameBufferObject1.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // needed for renderer
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);


    glBlendFunc(GL_ONE, GL_ZERO);
    for(int i = 0; i < mLights.size(); i++)
    {

      SetActiveShader(&mLightShader);
      mLights[i]->Bind();
      scene->Draw(delta);
      glBlendFunc(GL_ONE, GL_ONE);
    }

    // needed for renderer
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LEQUAL);
//    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);


    if(mPS != NULL)
      mPS->Render(delta);


    mFrameBufferObject1.UnBind();




    glViewport(0,0,1024,768);
    mFrameBufferObject1.BindTexture(FrameBuffer::COLOR_BUFFER_TEXTURE, GL_TEXTURE0);
    int loc = glGetUniformLocation(mDisplayShader.GetProgramID(), "texture");
    glUniform1i(loc, 0);

    SetActiveShader(&mDisplayShader);
    mDisplayVBO.Render();




//    std::cout.flush();
  }
}
