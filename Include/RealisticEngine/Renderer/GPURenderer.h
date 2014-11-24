#ifndef GPURENDERER_H
#define GPURENDERER_H

#include <vector>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "RealisticEngine/Renderer/BaseRenderer.h"
#include "RealisticEngine/Renderer/Shader.h"
#include "RealisticEngine/Renderer/Buffer.h"
#include "RealisticEngine/Renderer/FrameBuffer.h"
#include "RealisticEngine/Renderer/Asset.h"

namespace RealisticEngine
{
  namespace Renderer
  {
    class ParticleSystem;
    class Light;

    class GPURenderer : public BaseRenderer
    {
    public:

      virtual void Initialize();
      virtual void RenderScene(Scene::Node* scene, double delta);


      void AddLight(Light* light) { mLights.push_back(light); }

      void SetActiveShader(Shader* shader) { shader->MakeActive(); mActiveShader = shader; }

      void SetParticleSystem(ParticleSystem* ps) { mPS = ps; }


      Shader* GetActiveShader() { return mActiveShader; }
    protected:

      Shader* mActiveShader;

      std::vector<Light*> mLights;

      Shader mLightShader; // shader
      Shader mDisplayShader;
      Shader mDepthCaptureShader;
      GLuint mFramebufferName = 0;
      // The texture we're going to render to
      GLuint mRenderedTexture;
      // The depth buffer
      GLuint mDepthrenderbuffer;
      GLuint mDepthrenderTexture;

      VertexBufferObject mDisplayVBO;

      AttributeArrayBuffer mDisplayPositions;
      ElementArrayBuffer mDisplayIndices;
      FrameBuffer mFrameBufferObject1;

      ParticleSystem* mPS;



      FrameBuffer mFrameBufferObject2;

      UniformVariable mLightMatrix;
      UniformVariable mTexUnit;

    };
  }
}

#endif // GPURENDERER_H
