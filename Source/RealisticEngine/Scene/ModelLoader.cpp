#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "RealisticEngine/Scene/ModelLoader.h"
#include "RealisticEngine/Renderer/Asset.h"
#include "RealisticEngine/Renderer/DrawInterface.h"
#include "RealisticEngine/Renderer/Buffer.h"


#include "RealisticEngine/Utility/Utility.h"
#include <FreeImage.h>



using namespace RealisticEngine::Renderer;
using namespace RealisticEngine::Scene;
using namespace Assimp;



AssimpModelLoader::AssimpModelLoader()
{
  mImporter = new Importer();
}
AssimpModelLoader::~AssimpModelLoader()
{

  for(std::map<std::string, Texture*>::iterator it = mTextures.begin(); it != mTextures.end(); it++)
  {
    mTextures.erase(it);
    delete it->second;
  }
  for(std::map<std::string, const aiScene*>::iterator it = mLoadedModels.begin(); it != mLoadedModels.end(); it++)
  {
    mLoadedModels.erase(it);
//    delete it->second; // deleted by mImporter.~Importer()
  }
  delete mImporter;

}

void AssimpModelLoader::TraverseTree(const aiScene* scene, aiNode* ainode, Node* node, GPURenderer* renderer, std::string path)
{
  // create new node
  Node* subnode = new Node();
  node->AddChild(subnode);

  // update the model and normal matrices in the shader
  UpdateModelMatrix* updater = new UpdateModelMatrix();
  updater->Setup(subnode, renderer, "modelMat", "normalMat");
  subnode->AddAsset(updater);


  // create a variable to tell the shader which textureUnit the diffuseMap is loaded into.
  UniformVariable* diffuseTexUnit = new UniformVariable();
  diffuseTexUnit->Setup("diffTexture", UniformVariable::INT, 1);
  GLint data0 = 0;
  diffuseTexUnit->SetData(&data0, sizeof(GLint));
  diffuseTexUnit->SetRenderer(renderer);
  subnode->AddAsset(diffuseTexUnit);

  // create a variable to tell the shader which textureUnit the normalMap is loaded into
  UniformVariable* normalTexUnit = new UniformVariable();
  normalTexUnit->Setup("normTexture", UniformVariable::INT, 1);
  GLint data1 = 1;
  normalTexUnit->SetData(&data1, sizeof(GLint));
  normalTexUnit->SetRenderer(renderer);
  subnode->AddAsset(normalTexUnit);

  // convert the assimp transform matrix to an opengl transform matrix(swap row/col order apparently)
  glm::mat4 transform = glm::mat4(1);
  transform[0][0] = ainode->mTransformation[0][0];
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      transform[i][j] = ainode->mTransformation[j][i];

  // set the node's transform which will eventually propogate to the shader's transform matrices.
  subnode->SetLocalTransform(transform);

  // for all meshes attached to this node
  for(int i = 0; i < ainode->mNumMeshes; i++)
  {

    // create a vertexBufferObject to send attribute arrays to the shaders
    VertexBufferObject* vbo = new VertexBufferObject();
    vbo->SetRenderer(renderer);

    // create a DrawVertexBufferObject interface to actually make the draw call
    DrawVertexBufferObject* drawvbo = new DrawVertexBufferObject();
    drawvbo->SetVBO(vbo);
    subnode->AddDrawInterface(drawvbo);

    // get next mesh associated with this node
    aiMesh* mesh = scene->mMeshes[ainode->mMeshes[i]];
    // get next material associated with the next mesh
    aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

    // create a variable to tell the shader if their is a diffuse texture
    UniformVariable* hasDiffuseTex = new UniformVariable();
    hasDiffuseTex->Setup("hasDiffuseTex", UniformVariable::INT, 1);
    hasDiffuseTex->SetRenderer(renderer);
    subnode->AddAsset(hasDiffuseTex);

    // if the model expects to have a diffuse map
    if(mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
      // find the relative path of the diffuse map
      aiString Path;
      mat->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL);

      // find the path relative to the working directory
      std::string dir = path + '/';
      dir += Path.data;

      // load the texture
      Texture* texture = mTextures[dir]; // get original texture
      // check if texture has already been loaded
      if(texture)
      {
        std::cout << "diffuse " << dir << " -found" << std::endl;
        texture = new Texture(texture); // copy the original texture
      }
      if(texture == NULL)
      {
        std::cout << "diffuse " << dir << " -loaded" << std::endl;
        texture = RealisticEngine::Utility::LoadTextureFromFile(dir, GL_TEXTURE0, GL_UNSIGNED_BYTE);
        if(texture)
        {
          mTextures[dir] = texture; // store original texture
          texture = new Texture(texture); // make a copy to put in the node
        }
      }

      if(texture)
      {
        // if success tell the shaders there is a diffuse map
        subnode->AddAsset(texture);
        GLint data = 1;
        hasDiffuseTex->SetData(&data, sizeof(GLint));
      }
      else
      {
        // if failed tell the shaders there is no diffuse map
        std::cout << "diffuse " << dir << " -not found" << std::endl;
        GLint data = 0;
        hasDiffuseTex->SetData(&data, sizeof(GLint));
      }
      std::cout.flush();
    }
    else
    {
      // if not expecting a diffuse map tell the shaders
      GLint data = 0;
      hasDiffuseTex->SetData(&data, sizeof(GLint));
    }

    // if the model expects to have a normal map
    UniformVariable* hasNormalTex = new UniformVariable();
    hasNormalTex->Setup("hasNormalTex", UniformVariable::INT, 1);
    hasNormalTex->SetRenderer(renderer);
    subnode->AddAsset(hasNormalTex);
    if(mat->GetTextureCount(aiTextureType_NORMALS))
    {
      // find the relative path of the normal map
      aiString Path;
      mat->GetTexture(aiTextureType_NORMALS, 0, &Path, NULL, NULL, NULL, NULL, NULL);

      // find the path relative to the working directory
      std::string dir = path + '/';
      dir += Path.data;

      // load the texture
      Texture* texture = mTextures[dir]; // get original texture
      // check if texture has already been loaded
      if(texture)
      {
        std::cout << "diffuse " << dir << " -found" << std::endl;
        texture = new Texture(texture); // copy original texture
      }
      if(texture == NULL)
      {
        std::cout << "diffuse " << dir << " -loaded" << std::endl;
        texture = RealisticEngine::Utility::LoadTextureFromFile(dir, GL_TEXTURE0, GL_UNSIGNED_BYTE);
        if(texture)
        {
          mTextures[dir] = texture; // store original texture
          texture = new Texture(texture); // copy original texture
        }
      }
      if(texture)
      {
        // if success tell the shaders there is a normal map
        subnode->AddAsset(texture);
        std::cout << "normal " << dir << " -found" << std::endl;
        GLint data = 1;
        hasNormalTex->SetData(&data, sizeof(GLint));
      }
      else
      {
        // if failed tell the shaders there is no normal map
        std::cout << "normal " << dir << " -not found" << std::endl;
        GLint data = 0;
        hasNormalTex->SetData(&data, sizeof(GLint));
      }
      std::cout.flush();

    }
    else
    {
      // if not expecting a normal map tell the shaders
      GLint data = 0;
      hasNormalTex->SetData(&data, sizeof(GLint));
    }

    // if has indices load into vbo
    if(mesh->HasFaces())
    {
      GLushort* index = new GLushort[mesh->mNumFaces * 3];
      for(int j = 0; j < mesh->mNumFaces; j++)
      {
        index[j * 3 + 0] = mesh->mFaces[j].mIndices[0];
        index[j * 3 + 1] = mesh->mFaces[j].mIndices[1];
        index[j * 3 + 2] = mesh->mFaces[j].mIndices[2];
      }

      ElementArrayBuffer* indexBuffer = new ElementArrayBuffer();
      indexBuffer->Setup(renderer, index, mesh->mNumFaces * 3);
      indexBuffer->Load();
      vbo->SetIndices(indexBuffer);
    }

    // if has vertex positions load them into the vbo
    if(mesh->HasPositions())
    {
      AttributeArrayBuffer* positionBuffer = new AttributeArrayBuffer();
      positionBuffer->Setup(renderer, "position", mesh->mVertices, GL_FLOAT, 12, mesh->mNumVertices, 3);
      positionBuffer->Load();
      vbo->AddAtributeArray(positionBuffer);
    }

    // if has vertex normals load them into the vbo
    if(mesh->HasNormals())
    {
      AttributeArrayBuffer* normalBuffer = new AttributeArrayBuffer();
      normalBuffer->Setup(renderer, "normal", mesh->mNormals, GL_FLOAT, 12, mesh->mNumVertices, 3);
      normalBuffer->Load();
      vbo->AddAtributeArray(normalBuffer);
    }

    // if has texture coordinates load them into the vbo
    if(mesh->HasTextureCoords(0))
    {
      AttributeArrayBuffer* texCoords = new AttributeArrayBuffer();
      texCoords->Setup(renderer, "texcoord", mesh->mTextureCoords[0], GL_FLOAT, 12, mesh->mNumVertices, 3);
      texCoords->Load();
      vbo->AddAtributeArray(texCoords);
    }

    // if has vertex tangents load them into the vbo
    if(mesh->HasTangentsAndBitangents())
    {
      AttributeArrayBuffer* tangentBuffer = new AttributeArrayBuffer();
      tangentBuffer->Setup(renderer, "tangent", mesh->mTangents, GL_FLOAT, 12, mesh->mNumVertices, 3);
      tangentBuffer->Load();
      vbo->AddAtributeArray(tangentBuffer);
    }

    // set fallback diffuse values for when texture not present for mesh
    aiColor3D color(0.6f,0.6f,0.6f);
    mat->Get(AI_MATKEY_COLOR_DIFFUSE,color);
    GLfloat diffusedata[] = {color.r, color.g, color.b};
    UniformVariable* uniform = new UniformVariable();
    uniform->SetData(diffusedata, sizeof(GLfloat) * 3);
    uniform->SetRenderer(renderer);
    uniform->Setup("udiffuse", UniformVariable::VEC3F, 1);
    subnode->AddAsset(uniform);
  }

  // do this for all of this node's children
  for(int i = 0; i < ainode->mNumChildren; i++)
  {
    TraverseTree(scene, ainode->mChildren[i], subnode, renderer, path);
  }
}

bool AssimpModelLoader::Load(std::string dir, std::string fileName, Node *node)
{

  std::string path = dir + "/" + fileName;

  std::cout << "searching for model " << path << std::endl;

  // check if scene has been loaded
  const aiScene* scene = mLoadedModels[path];

  // if scene has not been loaded
  if(scene == NULL)
  {
    scene = mImporter->ReadFile(path, aiProcessPreset_TargetRealtime_Fast);//aiProcessPreset_TargetRealtime_Fast has the configs you'll needai

    if(scene == NULL)
    {
      std::cout << "model " << path << " -error" << std::endl;
      return false;
    }
    std::cout << "model " << path << " -loaded" << std::endl;
    mLoadedModels[path] = scene;
  }
  else
  {
    std::cout << "model " << path << " -found" << std::endl;
  }

  std::cout << "fetching assets...\n";
  std::cout.flush();


  // build model tree
  TraverseTree(scene, scene->mRootNode, node, mRenderer, dir);
  std::cout << "finished.\n";
  return true;
}

// warning: if some objects arn't on the heap this will cause a crash
bool AssimpModelLoader::Destroy(Node* node)
{
  // free everything that was dynamically added to the node.
  for(int i = 0; i < node->GetChildren().size(); i++)
  {
    Destroy(node->GetChildren()[i]);
  }
  for(int i = 0; i < node->GetAssets().size(); i++)
  {
    delete node->GetAssets()[i];
  }
  for(int i = 0; i < node->GetActions().size(); i++)
  {
    delete node->GetActions()[i];
  }
  for(int i = 0; i < node->GetDrawInterfaces().size(); i++)
  {
    delete node->GetDrawInterfaces()[i];
  }
  delete node;

  return false;
}
