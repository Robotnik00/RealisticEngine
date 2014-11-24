#include <glm/glm.hpp>

#include <RealisticEngine/Core/Engine.h>
#include <RealisticEngine/Core/SDLState.h>
#include <RealisticEngine/Renderer/GPURenderer.h>
#include <RealisticEngine/Scene/Node.h>
#include <RealisticEngine/Scene/ModelLoader.h>
#include <RealisticEngine/Scene/Camera.h>
#include <RealisticEngine/Scene/PhysxEngine.h>
#include <RealisticEngine/Renderer/ParticleSystem.h>

#define MAX_PARTICLES 1000000

namespace Projects
{
  namespace Demo
  {
    namespace RealisticWater
    {
      class RealisticWaterState : public RealisticEngine::Core::SDLState
      {
      public:
        RealisticWaterState(RealisticEngine::Core::Engine* engine) { mEngine = engine; }

        virtual void Initialize();

        virtual void Update();

        virtual void Render(double delta);

        virtual void ProcessEvent(SDL_Event env);

      protected:
        RealisticEngine::Core::Engine* mEngine;
        RealisticEngine::Renderer::GPURenderer mGPURenderer;
        RealisticEngine::Scene::Node mRootNode; // root of scene
        RealisticEngine::Scene::AssimpModelLoader mModelLoader;
        RealisticEngine::Scene::Camera mCamera;

        RealisticEngine::Scene::PhysxEngine mPhysx;


        RealisticEngine::Renderer::ParticleSystem mPS;

        GLfloat mPositions[MAX_PARTICLES];

        uint32_t mNumParticles;

        bool mKeysPressed[256];
        double mLastTime = 0;
        glm::vec4 mMouseVec;
      };
    }
  }
}
