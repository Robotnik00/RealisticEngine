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

#include <FreeImage.h>



using namespace RealisticEngine::Renderer;
using namespace RealisticEngine::Scene;
using namespace Assimp;



AssimpModelLoader::AssimpModelLoader()
{
}

void TraverseTree(const aiScene* scene, aiNode* ainode, Node* node, GPURenderer* renderer, std::string path)
{
  Node* subnode = new Node();
  node->AddChild(subnode);

  UniformVariable* modelMat = new UniformVariable();
  modelMat->Setup("modelMat", UniformVariable::MAT4F, 1);
  modelMat->BindActiveShader(renderer);
  modelMat->BindData(glm::value_ptr(*subnode->GetGlobalInterpolator()));
  subnode->AddAsset(modelMat);

  UniformVariable* normalMat = new UniformVariable();
  normalMat->Setup("normalMat", UniformVariable::MAT3F, 1);
  normalMat->BindActiveShader(renderer);
  normalMat->BindData(glm::value_ptr(*subnode->GetNormalMatrix()));
  subnode->AddAsset(normalMat);

  UniformVariable* diffuseTexUnit = new UniformVariable();
  diffuseTexUnit->Setup("diffTexture", UniformVariable::INT, 1);
  GLint* data = new GLint[1];
  data[0] = 0;
  diffuseTexUnit->BindData(data);
  diffuseTexUnit->BindActiveShader(renderer);
  subnode->AddAsset(diffuseTexUnit);

  UniformVariable* normalTexUnit = new UniformVariable();
  normalTexUnit->Setup("normTexture", UniformVariable::INT, 1);
  GLint* data1 = new GLint[1];
  data1[0] = 1;
  normalTexUnit->BindData(data1);
  normalTexUnit->BindActiveShader(renderer);
  subnode->AddAsset(normalTexUnit);

  glm::mat4 transform = glm::mat4(1);
  transform[0][0] = ainode->mTransformation[0][0];
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      transform[i][j] = ainode->mTransformation[j][i];

  subnode->SetLocalTransform(transform);

  for(int i = 0; i < ainode->mNumMeshes; i++)
  {

    VertexBufferObject* vbo = new VertexBufferObject();
    vbo->SetRenderer(renderer);

    DrawVertexBufferObject* drawvbo = new DrawVertexBufferObject();
    drawvbo->SetVBO(vbo);
    subnode->AddDrawInterface(drawvbo);

    aiMesh* mesh = scene->mMeshes[ainode->mMeshes[i]];
    aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

    if(mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
      aiString Path;
      mat->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL);

      std::string dir = path + '/';
      dir += Path.data;

      std::cout << dir << std::endl;
      std::cout.flush();


      FIBITMAP *dib = FreeImage_Load(FreeImage_GetFileType(dir.data()), dir.data(), FreeImage_GetFileType(dir.data()));
      if(dib != NULL)
      {
        std::cout << "it worked\n";
      }
      else
      {
        std::cout << "it failed\n";
      }

      GLenum type = GL_RGB;

      if(FreeImage_GetChannel(dib, FICC_ALPHA) != NULL)
      {
        type = GL_RGBA;
      }


      Texture* texture = new Texture();
      texture->Setup(FreeImage_GetBits(dib), FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), GL_TEXTURE0, type, GL_UNSIGNED_BYTE);
      texture->Load();
      subnode->AddAsset(texture);
      FreeImage_Unload(dib);
    }

    if(mat->GetTextureCount(aiTextureType_NORMALS))
    {
      aiString Path;

      mat->GetTexture(aiTextureType_NORMALS, 0, &Path, NULL, NULL, NULL, NULL, NULL);

      std::string dir = path + '/';
      dir += Path.data;

      std::cout << "normals " << dir << std::endl;

      FIBITMAP *dib = FreeImage_Load(FreeImage_GetFileType(dir.data()), dir.data(), FreeImage_GetFileType(dir.data()));
      if(dib == NULL)
      {
        std::cout << "failed\n";
      }
      else
      {
        std::cout << "success\n";
      }
      std::cout.flush();

      GLenum type = GL_RGB;

      if(FreeImage_GetChannel(dib, FICC_ALPHA) != NULL)
      {
        type = GL_RGBA;
      }

      Texture* texture = new Texture();
      texture->Setup(FreeImage_GetBits(dib), FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), GL_TEXTURE1, type, GL_UNSIGNED_BYTE);
      texture->Load();
      subnode->AddAsset(texture);
      FreeImage_Unload(dib);

      UniformVariable* hasNormalTex = new UniformVariable();
      hasNormalTex->Setup("hasNormalTex", UniformVariable::INT, 1);
      hasNormalTex->BindActiveShader(renderer);
      GLint* data = new GLint[1];
      data[0] = 1;
      hasNormalTex->BindData(data);
      subnode->AddAsset(hasNormalTex);
    }
    else
    {
      UniformVariable* hasNormalTex = new UniformVariable();
      hasNormalTex->Setup("hasNormalTex", UniformVariable::INT, 1);
      hasNormalTex->BindActiveShader(renderer);
      GLint* data = new GLint[1];
      data[0] = 0;
      hasNormalTex->BindData(data);
      subnode->AddAsset(hasNormalTex);
    }

    if(mesh->HasTextureCoords(0))
    {
      AttributeArrayBuffer* texCoords = new AttributeArrayBuffer();
      texCoords->Setup(renderer, "texcoord", mesh->mTextureCoords[0], GL_FLOAT, 12, mesh->mNumVertices, 3);
      texCoords->Load();
      vbo->AddAtributeArray(texCoords);
    }

    if(mesh->HasPositions())
    {
      AttributeArrayBuffer* positionBuffer = new AttributeArrayBuffer();
      positionBuffer->Setup(renderer, "position", mesh->mVertices, GL_FLOAT, 12, mesh->mNumVertices, 3);
      positionBuffer->Load();
      vbo->AddAtributeArray(positionBuffer);
    }

    if(mesh->HasNormals())
    {
      AttributeArrayBuffer* normalBuffer = new AttributeArrayBuffer();
      normalBuffer->Setup(renderer, "normal", mesh->mNormals, GL_FLOAT, 12, mesh->mNumVertices, 3);
      normalBuffer->Load();
      vbo->AddAtributeArray(normalBuffer);
    }

    if(mesh->HasTangentsAndBitangents())
    {
      std::cout << "has tangents\n";

      std::cout.flush();
      AttributeArrayBuffer* tangentBuffer = new AttributeArrayBuffer();
      tangentBuffer->Setup(renderer, "tangent", mesh->mTangents, GL_FLOAT, 12, mesh->mNumVertices, 3);
      tangentBuffer->Load();
      vbo->AddAtributeArray(tangentBuffer);
    }

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



    GLfloat* diffusedata = new GLfloat[3];
    diffusedata[0] = .6;
    diffusedata[1] = .6;
    diffusedata[2] = .6;

    UniformVariable* diffuse = new UniformVariable();
    diffuse->Setup("udiffuse", UniformVariable::VEC3F, 1);
    diffuse->BindData(diffusedata);
    diffuse->BindActiveShader(renderer);
    subnode->AddAsset(diffuse);

  }

  for(int i = 0; i < ainode->mNumChildren; i++)
  {
    TraverseTree(scene, ainode->mChildren[i], subnode, renderer, path);
  }
}

bool AssimpModelLoader::Load(std::string dir, std::string fileName, Node *node)
{
  Importer importer;

  std::string path = dir + "/" + fileName;
  const aiScene *scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Fast);//aiProcessPreset_TargetRealtime_Fast has the configs you'll needai

  if(scene == NULL)
  {
    std::cout << "cant load model\n";
    std::cout.flush();
    return false;
  }

  TraverseTree(scene, scene->mRootNode, node, mRenderer, dir);


  return false;
}

bool AssimpModelLoader::Destroy(std::string fileName)
{
  /*
   * To Do
   */
  return false;
}
