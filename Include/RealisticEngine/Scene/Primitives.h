#ifndef PRIMITIVES_H
#define PRIMITIVES_H

namespace RealisticEngine
{
  namespace Renderer
  {
    class GPURenderer;
  }
  namespace Scene
  {
    class Node;
    Node* CreateCube(float w, float h, float l, Renderer::GPURenderer* renderer);
    Node* CreateSphere(float r, int hsects, int vsects, Renderer::GPURenderer* renderer);
  }
}

#endif // PRIMITIVES_H
