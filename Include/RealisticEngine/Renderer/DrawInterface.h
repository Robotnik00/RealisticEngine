#ifndef DRAWINTERFACE_H
#define DRAWINTERFACE_H

#include <glm/glm.hpp>

namespace RealisticEngine
{
  namespace Renderer
  {
    class Buffer;
    class VertexBufferObject;
    class GPURenderer;


    class DrawInterface
    {
    public:
      virtual void Draw() = 0;
    };

    class DrawVertexBufferObject : public DrawInterface
    {
    public:
      DrawVertexBufferObject() { mVBO = NULL; }
      DrawVertexBufferObject(VertexBufferObject* vbo);

      void SetVBO(VertexBufferObject* vbo) { mVBO = vbo; }

      virtual void Draw();

      VertexBufferObject* mVBO;
    };


  }
}

#endif // DRAWINTERFACE_H
