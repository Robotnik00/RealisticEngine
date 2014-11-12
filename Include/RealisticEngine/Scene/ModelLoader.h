#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <map>
#include<vector>

#include "RealisticEngine/Scene/Node.h"
#include "RealisticEngine/Renderer/GPURenderer.h"

namespace RealisticEngine
{
  namespace Scene
  {
    class ModelLoader
    {
    public:

      virtual bool Load(std::string dir, std::string fileName, Node* node) = 0;
      virtual bool Destroy(std::string fileName) = 0;
    protected:
    };

    class AssimpModelLoader : public ModelLoader
    {
    public:
      AssimpModelLoader();
      void Setup(Renderer::GPURenderer* renderer) { mRenderer = renderer; }
      virtual bool Load(std::string dir, std::string fileName, Node* node);
      virtual bool Destroy(std::string fileName);
    protected:

      Renderer::GPURenderer* mRenderer;
    };
  }
}

#endif // MODELLOADER_H
