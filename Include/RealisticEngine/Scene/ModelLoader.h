#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <map>
#include<vector>

#include "RealisticEngine/Scene/Node.h"
#include "RealisticEngine/Renderer/GPURenderer.h"
#include "RealisticEngine/Renderer/Asset.h"
#include "RealisticEngine/Renderer/Buffer.h"

class aiScene;
class aiNode;

namespace Assimp
{
  class Importer;

}
namespace RealisticEngine
{
  namespace Scene
  {
    /*
     * loads code from an arbitrary file format.
     */
    class ModelLoader
    {
    public:

      virtual bool Load(std::string dir, std::string fileName, Node* node) = 0;
      virtual bool Destroy(Node* node) = 0;
    protected:
    };

    /*
     * an implementation of ModelLoader using assimp
     * for supported formats see http://assimp.sourceforge.net/
     */
    class AssimpModelLoader : public ModelLoader
    {
    public:
      AssimpModelLoader();
      ~AssimpModelLoader();

      void Setup(Renderer::GPURenderer* renderer) { mRenderer = renderer; }

      /*
       * Loads models from file
       *
       * The following data is loaded in
       * Uniforms:
       * 1. diffuse texture 'diffTexture'
       * 2. normal texture 'normTexture'
       * 3. fallback diffuse reflectance 'udiffuse'
       * 4. flag stating if their is a diffuse map 'hasDiffuseTex'
       * 5. flag stating if their is a normal map 'hasNormalTex'
       * 6. model matrix updater 'modelMat'
       * 7. normal matrix updater 'normalMat'.
       *
       * Attribute arrays
       * 1. vertex positions 'position'
       * 2. vertex normals 'normal'
       * 3. vertex tangents 'tangent'
       * 4. vertex indices. no name.
       */
      virtual bool Load(std::string dir, std::string fileName, Node* node);

      // deletes memory allocated to the node and all of its children
      virtual bool Destroy(Node* node);
    protected:
      void TraverseTree(const aiScene* scene, aiNode* ainode, Node* node, Renderer::GPURenderer* renderer, std::string path);

      Renderer::GPURenderer* mRenderer;

      Assimp::Importer* mImporter;

      std::map<std::string, Renderer::Texture*> mTextures;
      std::map<std::string, const aiScene*> mLoadedModels;
    };
  }
}

#endif // MODELLOADER_H
