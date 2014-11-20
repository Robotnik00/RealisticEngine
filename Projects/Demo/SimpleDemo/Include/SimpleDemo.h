#ifndef __SIMPLE_DEMO
#define __SIMPLE_DEMO

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <RealisticEngine/Core/Engine.h>
#include <RealisticEngine/Core/SDLState.h>
#include <RealisticEngine/Renderer/Shader.h>
#include <RealisticEngine/Renderer/Buffer.h>
#include <RealisticEngine/Scene/Node.h>
#include <RealisticEngine/Renderer/DrawInterface.h>
#include <RealisticEngine/Renderer/Asset.h>
#include <RealisticEngine/Scene/ModelLoader.h>
#include <RealisticEngine/Renderer/GPURenderer.h>

#include <RealisticEngine/Scene/Camera.h>
#include <RealisticEngine/Scene/PhysxEngine.h>

namespace Projects
{
  namespace Demo
  {
    namespace SimpleDemo 
    {
      class SimpleDemoState : public RealisticEngine::Core::SDLState
      {
      public:
        SimpleDemoState(RealisticEngine::Core::Engine* engine) { mEngine = engine; }   

        virtual void Initialize();
 
        virtual void Update();
        virtual void Render(double delta);

        virtual void ProcessEvent(SDL_Event env);
        
        RealisticEngine::Core::Engine* mEngine;
        RealisticEngine::Renderer::GPURenderer mGPURenderer;

        RealisticEngine::Scene::Node mRootNode; // root of scene
        RealisticEngine::Scene::Node mGimble;

        RealisticEngine::Renderer::UniformVariable mProjMat;
        RealisticEngine::Renderer::UniformVariable mViewMat;

        glm::mat4 mProjMatTransform;
        glm::mat4 mViewMatTransform; // view matrix

        RealisticEngine::Scene::AssimpModelLoader mModelLoader;
        RealisticEngine::Scene::Node* mnode1;
        RealisticEngine::Scene::Node* mnode2;
        RealisticEngine::Scene::Camera mCamera;

        RealisticEngine::Scene::PhysxEngine mPhysx;

        glm::vec4 mMouseVec;

        bool mKeysPressed[255];
        float mSpeed = 10.0;

        double lastTime;
      };
    }
  }
}

#endif
