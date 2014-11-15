#ifndef GPURENDERER_H
#define GPURENDERER_H

#include "RealisticEngine/Renderer/BaseRenderer.h"
#include "RealisticEngine/Renderer/Shader.h"

namespace RealisticEngine
{
  namespace Renderer
  {

    class GPURenderer : public BaseRenderer
    {
    public:

      virtual void Initialize();
      virtual void RenderScene(Scene::Node* scene, double delta);

      void SetActiveShader(Shader* shader) { shader->MakeActive(); mActiveShader = shader; }



      Shader* GetActiveShader() { return mActiveShader; }
    protected:

      Shader* mActiveShader;


      Shader mShader1; // shader
    };
  }
}

#endif // GPURENDERER_H
