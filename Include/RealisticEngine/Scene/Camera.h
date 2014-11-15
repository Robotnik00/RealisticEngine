#ifndef CAMERA_H
#define CAMERA_H

#include <string>

#include <glm/glm.hpp>

#include "RealisticEngine/Scene/Action.h"
#include "RealisticEngine/Scene/Node.h"
#include "RealisticEngine/Renderer/DrawInterface.h"
#include "RealisticEngine/Renderer/Asset.h"

namespace RealisticEngine
{
  namespace Renderer
  {
    class GPURenderer;
  }
  namespace Scene
  {
    class Camera : public Renderer::Asset, public Action
    {
    public:
      void Setup(std::string cameraPosName, std::string viewMatName, std::string projMatName, Renderer::GPURenderer* renderer);

      void PerspectiveMatrix(float fov, float width, float height, float near, float far);

      virtual void Bind();
      virtual void UnBind();

      virtual void PerformAction();


      void SetRotationMatrix(glm::mat4 rotmat);

      void SetTranslationMatrix(glm::mat4 transmat);

      void SetSceneNode(Node* node);

      void SetPosition(float x, float y, float z);
      void SetOrientation(float angle, glm::vec3 axis);

      glm::vec4 GetCamX();
      glm::vec4 GetCamY();
      glm::vec4 GetCamZ();
      glm::vec4 GetCamP();

      glm::mat4 GetMatrix();

      void Rotate(float angle, glm::vec3 axis);
      void Translate(glm::vec3 translation);


    protected:


      glm::vec4 mCamX;
      glm::vec4 mCamY;
      glm::vec4 mCamZ;
      glm::vec4 mCamP;

      glm::mat4 mCameraTransform;
      glm::mat4 mPrevCameraTransorm;


      Node* mNode;

      Renderer::UniformVariable mProjMatrix;
      Renderer::UniformVariable mViewMatrix;

      Renderer::UniformVariable mCameraPosition;

    };

  }
}

#endif // CAMERA_H
