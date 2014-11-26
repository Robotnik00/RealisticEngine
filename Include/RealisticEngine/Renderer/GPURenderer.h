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

      /*
       * all renderers do this
       */

      /*
       * initailze renderer
       */
      virtual void Initialize();

      /*
       * implementation specific way to render scene given arbitrary data in a scene
       */
      virtual void RenderScene(Scene::Node* scene, double delta);


      /*
       * only some renderers do this
       */

      /*
       * add a light source
       */
      void AddLight(Light* light) { mLights.push_back(light); }

      /*
       * other objects reference the shader to get for example... uniform location.
       * this is shader specific so these objects need to know the current active shader
       */
      void SetActiveShader(Shader* shader) { shader->MakeActive(); mActiveShader = shader; }

      /*
       * this particular renderer is capable of renderer a single particle system
       */
      void SetParticleSystem(ParticleSystem* ps) { mPS = ps; }


      Shader* GetActiveShader() { return mActiveShader; }
    protected:

      Shader* mActiveShader;

      std::vector<Light*> mLights;

      Shader mLightShader; // shader
      Shader mDisplayShader;
      Shader mDepthCaptureShader;
      // The texture we're going to render to
      GLuint mRenderedTexture;
      // The depth buffer
      GLuint mDepthrenderbuffer;
      GLuint mDepthrenderTexture;

      VertexBufferObject mDisplayVBO;

      AttributeArrayBuffer mDisplayPositions;
      ElementArrayBuffer mDisplayIndices;
      FrameBuffer mFrameBufferObject1;
      FrameBuffer mFrameBufferObject2;

      ParticleSystem* mPS;

      UniformVariable mLightMatrix;
      UniformVariable mTexUnit;

    };
  }
}

#endif // GPURENDERER_H
