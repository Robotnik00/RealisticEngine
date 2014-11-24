#include <string.h>

#include "RealisticEngine/Renderer/Buffer.h"
#include "RealisticEngine/Renderer/Shader.h"
#include "RealisticEngine/Renderer/GPURenderer.h"


using namespace RealisticEngine::Renderer;

Buffer::Buffer(GPURenderer* renderer, char* name, void* data, GLenum type, GLsizei bytesPerVertex, GLsizei nVertices, GLsizei dimensions)
  : mRenderer(renderer), mName(name), mType(type), mBytesPerVertex(bytesPerVertex), mNumVertices(nVertices), mDimensions(dimensions)
{
  mData = new char[nVertices * bytesPerVertex];
  memcpy(mData, data, nVertices * bytesPerVertex);
}

Buffer::~Buffer()
{
  glDeleteBuffers(1, &mID);
  if(mData != NULL)
    delete[] (char*)mData;
}

void Buffer::Setup(GPURenderer* renderer, char* name, void* data, GLenum type, GLsizei bytesPerVertex, GLsizei nVertices, GLsizei dimensions)
{
  mRenderer = renderer;
  mName = name;
  mData = new char[nVertices * bytesPerVertex];
  memcpy(mData, data, nVertices * bytesPerVertex);
  mType = type;
  mBytesPerVertex = bytesPerVertex;
  mNumVertices = nVertices;
  mDimensions = dimensions;
}

void Buffer::Delete()
{
  glDeleteBuffers(1, &mID);
}


ElementArrayBuffer::ElementArrayBuffer(GPURenderer* renderer, void *data, GLsizei nIndices)
  : Buffer::Buffer(renderer, NULL, data, GL_UNSIGNED_INT, 4, nIndices, 1)
{

}

void ElementArrayBuffer::Setup(GPURenderer *renderer, void *data, GLsizei nIndices)
{
  Buffer::Setup(renderer, NULL, data, GL_UNSIGNED_INT, 4, nIndices, 1);
}

void ElementArrayBuffer::Load()
{
  glGenBuffers(1, &mID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mBytesPerVertex * mNumVertices, mData, GL_STATIC_DRAW);
}

void ElementArrayBuffer::Bind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
}

AttributeArrayBuffer::AttributeArrayBuffer(GPURenderer *renderer, char* name, void *data, GLenum type, GLsizei bytesPerVertex, GLsizei nVertices, GLsizei dimensions)
  : Buffer::Buffer(renderer, name, data, type, bytesPerVertex, nVertices, dimensions)
{

}


void AttributeArrayBuffer::Setup(GPURenderer* renderer, char* name, void* data, GLenum type, GLsizei bytesPerVertex, GLsizei nVertices, GLsizei dimensions)
{
  Buffer::Setup(renderer, name, data, type, bytesPerVertex, nVertices, dimensions);
}

void AttributeArrayBuffer::Load()
{
  glGenBuffers(1, &mID);
  glBindBuffer(GL_ARRAY_BUFFER, mID);
  glBufferData(GL_ARRAY_BUFFER, mBytesPerVertex * mNumVertices, mData, GL_STREAM_DRAW);
}

void AttributeArrayBuffer::Update(void *data, GLint numVertices)
{
  mNumVertices = numVertices;
  glBindBuffer(GL_ARRAY_BUFFER, mID);
  glBufferData(GL_ARRAY_BUFFER, mBytesPerVertex * mNumVertices, data, GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, mBytesPerVertex * mNumVertices, data);
}

GLint AttributeArrayBuffer::GetLocation()
{
  GLint loc = glGetAttribLocation(mRenderer->GetActiveShader()->GetProgramID(), (GLchar*)mName);
  return loc;
}

void AttributeArrayBuffer::Bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, mID);
  GLint loc = glGetAttribLocation(mRenderer->GetActiveShader()->GetProgramID(), (GLchar*)mName);
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, mDimensions, GL_FLOAT, GL_FALSE, mBytesPerVertex, 0);
}

void VertexBufferObject::Render()
{
  for(int i = 0; i < mAttributeArrays.size(); i++)
  {
    mAttributeArrays[i]->Bind();
  }

  if(mIndexBuffer != NULL)
  {
    mIndexBuffer->Bind();
    glDrawElements(GL_TRIANGLES, mIndexBuffer->GetNumVertices(), GL_UNSIGNED_INT, 0);
  }
}

