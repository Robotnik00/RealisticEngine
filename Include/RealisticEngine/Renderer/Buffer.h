#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>



namespace RealisticEngine
{
  namespace Renderer
  {
    class Shader;
    class GPURenderer;

    class Buffer
    {
    public:
      Buffer() {}
      Buffer(GPURenderer* renderer, char* name, void* data, GLenum type, GLsizei bytesPerVertex, GLsizei nVertices, GLsizei dimensions);
      ~Buffer();


      virtual void Setup(GPURenderer* renderer, char* name, void* data, GLenum type, GLsizei bytesPerVertex, GLsizei nVertices, GLsizei dimensions);

      virtual void Delete();

      virtual void Load() = 0;
      virtual void Bind() = 0;

      GLint GetID() { return mID; }
      GLint GetNumVertices() { return mNumVertices; }


    protected:
      GLsizei mBytesPerVertex;
      GLsizei mNumVertices;
      GLsizei mDimensions;
      GLuint mID;
      GLenum mType;
      void* mData;
      char* mName;
      GPURenderer* mRenderer;
    };

    class ElementArrayBuffer : public Buffer
    {
    public:
      ElementArrayBuffer() {}
      ElementArrayBuffer(GPURenderer *renderer, void *data, GLsizei nIndices);
      virtual void Setup(GPURenderer *renderer, void *data, GLsizei nIndices);
      virtual void Load();
      virtual void Bind();

    };

    class AttributeArrayBuffer : public Buffer
    {
    public:
      AttributeArrayBuffer() {}
      AttributeArrayBuffer(GPURenderer* renderer, char* name, void* data, GLenum type, GLsizei bytesPerVertex, GLsizei nVertices, GLsizei dimensions);
      virtual void Setup(GPURenderer* renderer, char* name, void* data, GLenum type, GLsizei bytesPerVertex, GLsizei nVertices, GLsizei dimensions);
      virtual void Load();
      virtual void Bind();

    };

    class VertexBufferObject
    {
    public:
      VertexBufferObject() {}
      VertexBufferObject(GPURenderer* renderer) : mRenderer(renderer) {}

      void Render();
      void AddAtributeArray(Buffer* buffer) { mAttributeArrays.push_back(buffer); }
      void SetIndices(Buffer* buffer) { mIndexBuffer = buffer; }

      void SetRenderer(GPURenderer* renderer) { mRenderer = renderer; }

    protected:

      GPURenderer* mRenderer;

      Buffer* mIndexBuffer;
      std::vector<Buffer*> mAttributeArrays;
    };

  }
}

#endif // BUFFER_H
