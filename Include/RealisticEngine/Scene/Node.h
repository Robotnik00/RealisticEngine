#ifndef NODE_H
#define NODE_H


#include <vector>

#include <glm/glm.hpp>

#include "RealisticEngine/Scene/Action.h"


namespace RealisticEngine
{
  namespace Renderer
  {
    class DrawInterface;
    class Asset;
  }
  namespace Scene
  {
    class Node
    {
    public:

      Node();

      void SetLocalTransform(glm::mat4 transform);

      glm::mat4 GetLocalTransform();
      glm::mat4 GetGlobalTransform();
      glm::mat4* GetGlobalInterpolator();
      glm::mat3* GetNormalMatrix();

      void SetParent(Node* parent);
      int AddChild(Node* child);
      int RemoveChild(Node* child);

      int AddAction(Action* action);
      int RemoveAction(Action* action);

      int AddDrawInterface(Renderer::DrawInterface* drawInterface);
      int RemoveDrawInterface(Renderer::DrawInterface* drawInterface);

      int AddAsset(Renderer::Asset* asset);
      int RemoveAsset(Renderer::Asset* asset);

      Node* GetParent();
      int GetChildren(Node** children);

      void Update();
      void Draw(float delta);

    protected:



      glm::mat4 mTransform;
      glm::mat4 mPrevGlobalTransform;
      glm::mat4 mInterpolator;
      glm::mat3 mNormalMatrix;

      Node* mParent;
      std::vector<Node*> mChildren;

      std::vector<Renderer::DrawInterface*> mDrawInterfaces;
      std::vector<Action*> mActions;
      std::vector<Renderer::Asset*> mAssets;
    };
  }
}

#endif // NODE_H
