#ifndef NODE_H
#define NODE_H


#include <vector>

#include <glm/glm.hpp>

#include "RealisticEngine/Scene/Action.h"

#include "RealisticEngine/Scene/Geomerty.h"

namespace RealisticEngine
{
  namespace Renderer
  {
    class DrawInterface;
    class Asset;
  }
  namespace Scene
  {
    /*
     * Nodes define a coordinate space for a set of
     * drawcalls, actions and geometry.
     *
     * build scenes in a tree structure using nodes.
     * every node is define with respect to its parent coordinate space
     *
     * add actions to nodes be updated every engine tick. Ex. Camera, Animation
     *
     * add drawinterfaces to the node to be renderer every frame. ex. Vertex Buffer Object
     * add assets to the node to be binded to before any drawinterfaces are called. ex. Texture, Uniform variable
     */
    class Node
    {
    public:

      Node();

      void SetLocalTransform(glm::mat4 transform);

      void Translate(glm::vec3 translation);
      void Rotate(float angle, glm::vec3 axis);

      glm::mat4 GetLocalTransform();
      glm::mat4 GetGlobalTransform();
      glm::mat4 GetGlobalInterpolator();
      glm::mat3 GetNormalMatrix();

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
      std::vector<Node*> GetChildren();
      std::vector<Action*> GetActions();
      std::vector<Renderer::Asset*> GetAssets();
      std::vector<Renderer::DrawInterface*> GetDrawInterfaces();

      void SetGeometry(Geometry geometry) { mGeometry = geometry; }
      Geometry GetGeometry() { return mGeometry; }

      void Update();
      void Draw(float delta);

      float GetDelta() { return mDelta; }

    protected:

      float mDelta;

      glm::mat4 mTransform;
      glm::mat4 mPrevGlobalTransform;
      glm::mat4 mInterpolator;
      glm::mat3 mNormalMatrix;


      Node* mParent;
      std::vector<Node*> mChildren;

      std::vector<Action*> mActions;
      std::vector<Renderer::Asset*> mAssets;
      std::vector<Renderer::DrawInterface*> mDrawInterfaces;

      Geometry mGeometry;

      bool mStartingPos;
    };
  }
}

#endif // NODE_H
