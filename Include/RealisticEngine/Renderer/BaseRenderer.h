#ifndef RENDERER_H
#define RENDERER_H


namespace RealisticEngine
{
  namespace Scene
  {
    class Node;
  }
  namespace Renderer
  {
    class BaseRenderer
    {
    public:
      BaseRenderer();

      virtual void Initialize() {}


      virtual void RenderScene(Scene::Node* scene, float delta) {}
    };
  }
}

#endif // RENDERER_H
