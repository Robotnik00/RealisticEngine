#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "RealisticEngine/Renderer/Shader.h"
#include "RealisticEngine/Renderer/Buffer.h"
#include "RealisticEngine/Scene/Camera.h"
#include "RealisticEngine/Renderer/GPURenderer.h"
#include "RealisticEngine/Utility/Utility.h"

namespace RealisticEngine
{
  namespace Renderer
  {
    struct Particle
    {
      glm::vec3 pos;
    };

    /*
     * this code is used to set up buffers for renderering
     * a particle system.
     * uses its own specailized data, so certain data from scene
     * must be given to it so it works properly ie. camera
     */
    class ParticleSystem
    {
    public:
      void Setup(Scene::Camera* camera, GPURenderer* renderer);
      void Initialize();
      void SetParticles(GLfloat* particleposbuffer, GLint numParticles);
      void Render(float delta);


    protected:
      std::vector<Particle*> mParticles;

      AttributeArrayBuffer mQuadBuffer;
      AttributeArrayBuffer mQuadPosition;
      ElementArrayBuffer mQuadIndices;


      AttributeArrayBuffer mPositions;

      GPURenderer* mRenderer;

      Shader mShader;
      Scene::Camera* mCamera;

      uint32_t mNumParticles;
      RealisticEngine::Utility::Sphere sphere;
    };
  }
}

#endif // PARTICLESYSTEM_H
