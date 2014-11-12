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
        RealisticEngine::Renderer::UniformVariable mProjMat; // uniform variable
        RealisticEngine::Renderer::UniformVariable mViewMat; //

        glm::mat4 mProjMatTransform; // projection matrix
        glm::mat4 mViewMatTransform; // view matrix
        glm::mat4 mRootNodeTransform; // rootNode matrix

        RealisticEngine::Renderer::Shader mShader1; // shader
        GLuint mActiveShader; // id of the current active shader


        RealisticEngine::Scene::AssimpModelLoader mModelLoader;

      };
    }
  }
}

#endif
