#include <FreeImage.h>
#include <vector>

#include "RealisticEngine/Utility/Utility.h"

#include "RealisticEngine/Renderer/Asset.h"

#define PI 3.141592654

using namespace RealisticEngine::Renderer;


Texture* RealisticEngine::Utility::LoadTextureFromFile(std::string filename, GLenum textureUnit, GLenum type)
{

  FIBITMAP *dib = FreeImage_Load(FreeImage_GetFileType(filename.data()), filename.data(), FreeImage_GetFileType(filename.data()));
  if(dib == NULL)
  {
    return NULL;
  }
  GLenum format = GL_RGB;
  if(FreeImage_GetChannel(dib, FICC_ALPHA) != NULL)
  {
    format = GL_RGBA;
  }

  Texture* texture = new Texture();
  texture->Setup(FreeImage_GetBits(dib), FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), textureUnit, format, type);
  texture->Load();
  FreeImage_Unload(dib);

  return texture;
}


RealisticEngine::Utility::Sphere RealisticEngine::Utility::CreateSphere(float r, int hsects, int vsects)
{
  float thetastep = PI / (hsects+1);
  float theta = 0;
  float phistep = 2*PI/vsects;
  float phi = 0;

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<GLushort> indices;

  glm::vec3 topposition = glm::vec3(0,0,r);
  glm::vec3 topnormal = glm::normalize(glm::vec3(0,0,r));

  positions.push_back(topposition);
  normals.push_back(topnormal);

  for(int i = 0; i < hsects; i++)
  {
    theta += thetastep;
    for(int j = 0; j < vsects; j++)
    {
      float x = cos(phi) * sin(theta) * r;
      float y = sin(phi) * sin(theta) * r;
      float z = cos(theta) * r;
      glm::vec3 pos = glm::vec3(x,y,z);
      glm::vec3 norm = glm::normalize(glm::vec3(x,y,z));
      positions.push_back(pos);
      normals.push_back(norm);
      phi += phistep;
    }
  }

  glm::vec3 bottomposition = glm::vec3(0,0,-r);
  glm::vec3 bottomnormal = glm::normalize(glm::vec3(0,0,-r));

  positions.push_back(bottomposition);
  normals.push_back(bottomnormal);

  //add top indices
  for(int i = 2; i < vsects+1; i++)
  {
      indices.push_back(0);
      indices.push_back(i-1);
      indices.push_back(i);
  }
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(vsects);


  for(int i = 0; i < hsects-1; i++)
  {
      for(int j = 0; j < vsects+1; j++)
      {
          indices.push_back(vsects*i+j);
          indices.push_back(vsects*i+j+1);
          indices.push_back(vsects*(i+1)+j);
      }
      for(int j = 0; j < vsects+1; j++)
      {
          indices.push_back(vsects*i+j+1);
          indices.push_back(vsects*(i+1)+j+1);
          indices.push_back(vsects*(i+1)+j);
      }
  }

  //add bottom indices
  for(int i = 1; i < vsects; i++)
  {
      indices.push_back(positions.size() - 1);
      indices.push_back(positions.size() - 1 - i);
      indices.push_back(positions.size() - 2 - i);
  }
  indices.push_back(positions.size() - 1);
  indices.push_back(positions.size() - 2);
  indices.push_back(positions.size() - vsects-1);

  GLfloat* positionbuffer = new GLfloat[positions.size()*3];
  GLfloat* normalbuffer = new GLfloat[normals.size()*3];

  for(int i = 0; i < positions.size(); i++)
  {
    for(int j = 0; j < 3; j++)
    {
      positionbuffer[i*3 + j] = positions[i][j];
      normalbuffer[i*3 + j] = normals[i][j];
    }
  }
  GLuint* indexbuffer = new GLuint[indices.size()];
  for(int i = 0; i < indices.size(); i++)
  {
    indexbuffer[i] = indices.data()[i];
  }

  RealisticEngine::Utility::Sphere sphere;
  sphere.indices = indexbuffer;
  sphere.positions = positionbuffer;
  sphere.normals = normalbuffer;
  sphere.numIndices = indices.size();
  sphere.numPos = positions.size();
  return sphere;

}
