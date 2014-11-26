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

    /*
     * an abstract drawcall of some kind
     */
    class DrawInterface
    {
    public:
      virtual void Draw() = 0;
    };


    /*
     * draws a vertex buffer object
     */
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
