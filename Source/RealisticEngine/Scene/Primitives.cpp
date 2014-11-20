#include <math.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RealisticEngine/Scene/Node.h"
#include "RealisticEngine/Renderer/Asset.h"

#include "RealisticEngine/Renderer/Buffer.h"
#include "RealisticEngine/Renderer/DrawInterface.h"
#include "RealisticEngine/Scene/Primitives.h"

using namespace RealisticEngine::Scene;
using namespace RealisticEngine::Renderer;

#define PI 3.141592654


Node* RealisticEngine::Scene::CreateCube(float w, float h, float l, GPURenderer* renderer)
{


  GLfloat positionbuffer[] = {
    // front
    -w/2,-h/2,l/2,
    w/2,-h/2,l/2,
    w/2,h/2,l/2,
    -w/2,h/2,l/2,

    // back
    -w/2,-h/2,-l/2,
    w/2,-h/2,-l/2,
    w/2,h/2,-l/2,
    -w/2,h/2,-l/2,

    // left
    -w/2,-h/2,l/2,
    -w/2,-h/2,-l/2,
    -w/2,h/2,-l/2,
    -w/2,h/2,l/2,

    // right
    w/2,-h/2,l/2,
    w/2,-h/2,-l/2,
    w/2,h/2,-l/2,
    w/2,h/2,l/2,

    // top
    -w/2,h/2,l/2,
    -w/2,h/2,-l/2,
    w/2,h/2,-l/2,
    w/2,h/2,l/2,

    // bottom
    -w/2,-h/2,l/2,
    -w/2,-h/2,-l/2,
    w/2,-h/2,-l/2,
    w/2,-h/2,l/2,

  };

  GLfloat normalbuffer[] = {
    // front
    0.0,0.0,1.0,
    0.0,0.0,1.0,
    0.0,0.0,1.0,
    0.0,0.0,1.0,

    // back
    0.0,0.0,-1.0,
    0.0,0.0,-1.0,
    0.0,0.0,-1.0,
    0.0,0.0,-1.0,

    // left
    -1.0,0.0,0.0,
    -1.0,0.0,0.0,
    -1.0,0.0,0.0,
    -1.0,0.0,0.0,

    // right
    1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,

    // top
    0.0,1.0,0.0,
    0.0,1.0,0.0,
    0.0,1.0,0.0,
    0.0,1.0,0.0,

    // bottom
    0.0,-1.0,0.0,
    0.0,-1.0,0.0,
    0.0,-1.0,0.0,
    0.0,-1.0,0.0,
  };

  GLushort indices[] = {
    // front
    0,1,2,
    0,2,3,

    // back
    4,5,6,
    4,6,7,

    // left
    8,9,10,
    8,10,11,

    // right
    12,13,14,
    12,14,15,

    // top
    16,17,18,
    16,18,19,

    // bottom
    20,21,22,
    20,22,23,
  };





  VertexBufferObject* vbo = new VertexBufferObject();

  AttributeArrayBuffer* positionbufferobject = new AttributeArrayBuffer();
  positionbufferobject->Setup(renderer, "position", positionbuffer, GL_FLOAT, sizeof(GLfloat) * 3, 24, 3);
  positionbufferobject->Load();
  AttributeArrayBuffer* normalbufferobject = new AttributeArrayBuffer();
  normalbufferobject->Setup(renderer, "normal", normalbuffer, GL_FLOAT, sizeof(GLfloat) * 3, 24, 3);
  normalbufferobject->Load();
  ElementArrayBuffer* indexbuffer = new ElementArrayBuffer();
  indexbuffer->Setup(renderer, indices, 36);
  indexbuffer->Load();
  vbo->AddAtributeArray(positionbufferobject);
  vbo->AddAtributeArray(normalbufferobject);
  vbo->SetIndices(indexbuffer);

  DrawVertexBufferObject* drawvbo = new DrawVertexBufferObject();
  drawvbo->SetVBO(vbo);
  Node* objectNode = new Node();
  objectNode->AddDrawInterface(drawvbo);

  UpdateModelMatrix* updater = new UpdateModelMatrix();
  updater->Setup(objectNode, renderer, "modelMat", "normalMat");
  objectNode->AddAsset(updater);


  Geometry geo;
  for(int i = 0; i < 24; i++)
  {
    glm::vec3 pos;
    pos.x = positionbuffer[i*3 + 0];
    pos.y = positionbuffer[i*3 + 1];
    pos.z = positionbuffer[i*3 + 2];
    geo.mVertices.push_back(pos);
  }

  objectNode->SetGeometry(geo);




  return objectNode;
}

Node* RealisticEngine::Scene::CreateSphere(float r, int hsects, int vsects, GPURenderer* renderer)
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

  std::cout << "positions " << positions.size() << "\n";
  std::cout.flush();

  VertexBufferObject* vbo = new VertexBufferObject();

  AttributeArrayBuffer* positionbufferobject = new AttributeArrayBuffer();
  positionbufferobject->Setup(renderer, "position", positionbuffer, GL_FLOAT, sizeof(GLfloat) * 3, positions.size(), 3);
  positionbufferobject->Load();
  AttributeArrayBuffer* normalbufferobject = new AttributeArrayBuffer();
  normalbufferobject->Setup(renderer, "normal", normalbuffer, GL_FLOAT, sizeof(GLfloat) * 3, normals.size(), 3);
  normalbufferobject->Load();
  ElementArrayBuffer* indexbuffer = new ElementArrayBuffer();
  indexbuffer->Setup(renderer, indices.data(), indices.size());
  indexbuffer->Load();
  vbo->AddAtributeArray(positionbufferobject);
  vbo->AddAtributeArray(normalbufferobject);
  vbo->SetIndices(indexbuffer);

  DrawVertexBufferObject* drawvbo = new DrawVertexBufferObject();
  drawvbo->SetVBO(vbo);
  Node* objectNode = new Node();
  objectNode->AddDrawInterface(drawvbo);

  UpdateModelMatrix* updater = new UpdateModelMatrix();
  updater->Setup(objectNode, renderer, "modelMat", "normalMat");
  objectNode->AddAsset(updater);

  return objectNode;
}

