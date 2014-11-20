#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RealisticEngine/Scene/Camera.h"

using namespace RealisticEngine::Scene;
using namespace RealisticEngine::Renderer;

void Camera::Setup(std::string cameraPosName, std::string viewMatName, std::string projMatName, GPURenderer* renderer)
{
  mViewMatrix.Setup(viewMatName, UniformVariable::MAT4F, 1);
  mViewMatrix.SetRenderer(renderer);

  mProjMatrix.Setup(projMatName, UniformVariable::MAT4F, 1);
  mProjMatrix.SetRenderer(renderer);

  mCameraPosition.Setup(cameraPosName, UniformVariable::VEC3F, 1);
  mCameraPosition.SetRenderer(renderer);

  mCamX = glm::vec4(1,0,0,0);
  mCamY = glm::vec4(0,1,0,0);
  mCamZ = glm::vec4(0,0,1,0);
  mCamP = glm::vec4(0,0,0,1);


  mNode = NULL;
}


void Camera::Bind()
{
  float delta = mNode->GetDelta();

  glm::mat4 interpolator = mCameraTransform * delta + mPrevCameraTransorm * (1-delta);

  mViewMatrix.SetData(glm::value_ptr(interpolator), sizeof(glm::mat4));
  mViewMatrix.Bind();

  glm::vec3 pos = glm::vec3(mCamP);
  mCameraPosition.SetData(glm::value_ptr(pos), sizeof(glm::vec3));
  mCameraPosition.Bind();

  mProjMatrix.Bind();
}

void Camera::UnBind()
{
  mViewMatrix.UnBind();
}

void Camera::PerformAction()
{
  mPrevCameraTransorm = mCameraTransform;
  mCameraTransform = glm::mat4(mCamX, mCamY, mCamZ, mCamP);
  mCameraTransform = glm::inverse(mCameraTransform);
}


void Camera::SetPosition(float x, float y, float z)
{
  mCamP = glm::vec4(x,y,z,1);
}

glm::vec4 Camera::GetCamX()
{
  return mCamX;
}
glm::vec4 Camera::GetCamY()
{
  return mCamY;
}
glm::vec4 Camera::GetCamZ()
{
  return mCamZ;
}
glm::vec4 Camera::GetCamP()
{
  return mCamP;
}

glm::mat4 Camera::GetMatrix()
{
  return glm::mat4(mCamX, mCamY, mCamZ, mCamP);
}

void Camera::Translate(glm::vec3 translation)
{
  mCamP.x += translation.x;
  mCamP.y += translation.y;
  mCamP.z += translation.z;
}

void Camera::Rotate(float angle, glm::vec3 axis)
{
  glm::mat4 rotMat = glm::mat4(1);
  rotMat = glm::rotate(rotMat, angle, axis);

  mCamX = glm::normalize(rotMat * mCamX);
  mCamY = glm::normalize(rotMat * mCamY);
  mCamZ = glm::normalize(rotMat * mCamZ);

}

void Camera::SetOrientation(float angle, glm::vec3 axis)
{
  glm::mat4 rotMat = glm::mat4(1);
  rotMat = glm::rotate(rotMat, angle, axis);
  mCamX = rotMat[0];
  mCamY = rotMat[1];
  mCamZ = rotMat[2];
}
void Camera::PerspectiveMatrix(float fov, float width, float height, float near, float far)
{
  glm::mat4 projMat = glm::perspectiveFov(fov, width, height, near, far);
  mProjMatrix.SetData(glm::value_ptr(projMat), sizeof(glm::mat4));
  mProjMatrix.Bind();
}


void Camera::SetSceneNode(Node *node)
{
  mNode = node;
}
