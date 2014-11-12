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
      virtual void RenderScene(Scene::Node* scene, float delta);

      void SetActiveShader(Shader* shader) { mActiveShader = shader; }
      Shader* GetActiveShader() { return mActiveShader; }
    protected:

      Shader* mActiveShader;
    };
  }
}

#endif // GPURENDERER_H
