#include <string.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "RealisticEngine/Renderer/Asset.h"
#include "RealisticEngine/Renderer/GPURenderer.h"

#include "RealisticEngine/Scene/Node.h"


using namespace RealisticEngine::Renderer;


UniformVariable::UniformVariable()
{
  mData = NULL;
}




UniformVariable::UniformVariable(std::string name, UniformType type, GLsizei count)
{
  mType = type;
  mName = name;
  mCount = count;
}

UniformVariable::~UniformVariable()
{
  if(mData != NULL)
  {
    delete[] mData;
  }
}

void UniformVariable::Setup(std::string name, UniformType type, GLsizei count)
{
  mName = name;
  mType = type;
  mCount = count;
}

void UniformVariable::Bind()
{
  GLint loc = glGetUniformLocation(mRenderer->GetActiveShader()->GetProgramID(), mName.data());
  if(loc < 0)
  {
    return;
  }


  switch(mType)
  {
    case INT:
    {
      GLint* data = (GLint*)mData;
      glUniform1i(loc, data[0]);
      break;
    }
    case INTARY:
    {
      GLint* data = (GLint*)mData;
      glUniform1iv(loc, mCount, data);
      break;
    }
    case VEC2I:
    {
      GLint* data = (GLint*)mData;
      glUniform2i(loc, data[0], data[1]);
      break;
    }
    case VEC2IARY:
    {
      GLint* data = (GLint*)mData;
      glUniform2iv(loc, mCount, data);
      break;
    }
    case VEC3I:
    {
      GLint* data = (GLint*)mData;
      glUniform3i(loc, data[0], data[1], data[2]);
      break;
    }
    case VEC3IARY:
    {
      GLint* data = (GLint*)mData;
      glUniform3iv(loc, mCount, data);
      break;
    }
    case VEC4I:
    {
      GLint* data = (GLint*)mData;
      glUniform4i(loc, data[0], data[1], data[2], data[3]);
      break;
    }
    case VEC4IARY:
    {
      GLint* data = (GLint*)mData;
      glUniform4iv(loc, mCount, data);
      break;
    }
    case FLOAT:
    {
      GLfloat* data = (GLfloat*)mData;
      glUniform1f(loc, data[0]);
      break;
    }
    case FLOATARY:
    {
      GLfloat* data = (GLfloat*)mData;
      glUniform1fv(loc, mCount, data);
      break;
    }
    case VEC2F:
    {
      GLfloat* data = (GLfloat*)mData;
      glUniform2f(loc, data[0], data[1]);
      break;
    }
    case VEC2FARY:
    {
      GLfloat* data = (GLfloat*)mData;
      glUniform2fv(loc, mCount, data);
      break;
    }
    case VEC3F:
    {
      GLfloat* data = (GLfloat*)mData;
      glUniform3f(loc, data[0], data[1], data[2]);
      break;
    }
    case VEC3FARY:
    {
      GLfloat* data = (GLfloat*)mData;
      glUniform3fv(loc, mCount, data);
      break;
    }
    case VEC4F:
    {
      GLfloat* data = (GLfloat*)mData;
      glUniform4f(loc, data[0], data[1], data[2], data[3]);
      break;
    }
    case VEC4FARY:
    {
      GLfloat* data = (GLfloat*)mData;
      glUniform4fv(loc, mCount, data);
      break;
    }
    case MAT2F:
    {
      GLfloat* data = (GLfloat*)mData;
      glUniformMatrix2fv(loc, mCount, false, data);
      break;
    }
    case MAT3F:
    {
      GLfloat* data = (GLfloat*)mData;
      glUniformMatrix3fv(loc, mCount, false, data);
      break;
    }
    case MAT4F:
    {
      GLfloat* data = (GLfloat*)mData;
      glUniformMatrix4fv(loc, mCount, false, data);
      break;
    }

    default:
      break;
  }
}

void UniformVariable::SetRenderer(GPURenderer *renderer)
{
  mRenderer = renderer;
}

void UniformVariable::SetData(void *data, uint16_t bytesize)
{
  if(mData != NULL)
  {
    delete[] mData;
  }
  mData = new char[bytesize];
  memcpy(mData, data, bytesize);
}

Texture::Texture(Texture* rhs)
{
  this->mWidth = rhs->mWidth;
  this->mHeight = rhs->mHeight;
  this->mFormat = rhs->mFormat;
  this->mPixleType = rhs->mPixleType;
  this->mTexid = rhs->mTexid;
  this->mTexUnit = rhs->mTexUnit;
  this->mData = NULL;
}

void Texture::Setup(void *data, GLuint width, GLuint height, GLenum texunit, GLenum format, GLenum pixletype)
{
  mData = data;
  mWidth = width;
  mHeight = height;
  mTexUnit = texunit;
  mFormat = format;
  mPixleType = pixletype;
}

void Texture::Load()
{
  glGenTextures(1, &mTexid);
  glBindTexture(GL_TEXTURE_2D,mTexid);
  glTexImage2D(GL_TEXTURE_2D, 0, mFormat, mWidth,mHeight, 0, mFormat, mPixleType, mData);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Unload()
{
  glDeleteTextures(1, &mTexid);
}

void Texture::Bind()
{
  glActiveTexture(mTexUnit);
  glBindTexture(GL_TEXTURE_2D, mTexid);
}

void Texture::UnBind()
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

void UpdateModelMatrix::Setup(Scene::Node *node, GPURenderer* renderer, std::string modelMatName, std::string normalMatName)
{
  mNode = node;

  mModelMat.Setup(modelMatName, UniformVariable::MAT4F, 1);
  mModelMat.SetRenderer(renderer);

  mNormalMat.Setup(normalMatName, UniformVariable::MAT3F, 1);
  mNormalMat.SetRenderer(renderer);
}

void UpdateModelMatrix::Bind()
{
  glm::mat4 data1 = mNode->GetGlobalInterpolator();
  mModelMat.SetData(glm::value_ptr(data1), sizeof(glm::mat4));
  mModelMat.Bind();
  glm::mat3 data2 = mNode->GetNormalMatrix();
  mNormalMat.SetData(glm::value_ptr(data2), sizeof(glm::mat3));
  mNormalMat.Bind();
}

void Light::Setup(std::string lightName, GPURenderer* renderer)
{
  mLightPosition.Setup(lightName + ".Position", UniformVariable::VEC3F, 1);
  mLightPosition.SetRenderer(renderer);

  mLightColor.Setup(lightName + ".I0", UniformVariable::VEC3F, 1);
  mLightColor.SetRenderer(renderer);

  float data = 10;
  mQuadraticAttenuation.Setup(lightName + ".QuadraticAttenuation", UniformVariable::FLOAT, 1);
  mQuadraticAttenuation.SetRenderer(renderer);
  mQuadraticAttenuation.SetData(&data, sizeof(float));

}

void  Light::Bind()
{
  mLightPosition.Bind();
  mLightColor.Bind();
  mQuadraticAttenuation.Bind();
}
void Light::UnBind()
{

}

void Light::SetColor(glm::vec3 color)
{
  mLightColor.SetData(glm::value_ptr(color), sizeof(glm::vec3));
}

void Light::SetPosition(glm::vec3 position)
{
  mPosition = position;
  mLightPosition.SetData(glm::value_ptr(position), sizeof(glm::vec3));
}

void Light::SetQuadraticAttenuation(float attenuation)
{
  mQuadraticAttenuation.SetData(&attenuation, sizeof(float));
}

void Material::Setup(std::string materialName, GPURenderer *renderer)
{
  glm::vec3 diff = glm::vec3(0.6,0.6,0.6);
  mDiffuseReflectance.Setup(materialName + ".KDiff", UniformVariable::VEC3F, 1);
  mDiffuseReflectance.SetRenderer(renderer);
  mDiffuseReflectance.SetData(glm::value_ptr(diff), sizeof(glm::vec3));


  glm::vec3 spec = glm::vec3(0.3,0.3,0.3);
  mSpecularReflectance.Setup(materialName + ".KSpec", UniformVariable::VEC3F, 1);
  mSpecularReflectance.SetRenderer(renderer);
  mSpecularReflectance.SetData(glm::value_ptr(spec), sizeof(glm::vec3));

  GLfloat shininess = 20.0;
  mShininess.Setup(materialName + ".Shininess", UniformVariable::FLOAT, 1);
  mShininess.SetRenderer(renderer);
  mShininess.SetData(&shininess, sizeof(GLfloat));


  GLint data = 0;
  mHasDiffuseTexture.Setup(materialName + ".HasDiffuseTex", UniformVariable::INT, 1);
  mHasDiffuseTexture.SetRenderer(renderer);
  mHasDiffuseTexture.SetData(&data, sizeof(GLint));

  mHasNormalTexture.Setup(materialName + ".HasNormalTex", UniformVariable::INT, 1);
  mHasNormalTexture.SetRenderer(renderer);
  mHasNormalTexture.SetData(&data, sizeof(GLint));

  glm::vec3 ambient = glm::vec3(0,0,0);
  mAmbient.Setup(materialName + ".IAmbient", UniformVariable::VEC3F, 1);
  mAmbient.SetRenderer(renderer);
  mAmbient.SetData(glm::value_ptr(ambient), sizeof(glm::vec3));

  GLint difftexunit = 0;
  mDiffuseTextureUnit.Setup(materialName + ".DiffuseTexture", UniformVariable::INT, 1);
  mDiffuseTextureUnit.SetRenderer(renderer);
  mDiffuseTextureUnit.SetData(&difftexunit, sizeof(GLint));

  GLint normtexunit = 1;
  mNormalTextureUnit.Setup(materialName + ".DiffuseTexture", UniformVariable::INT, 1);
  mNormalTextureUnit.SetRenderer(renderer);
  mNormalTextureUnit.SetData(&normtexunit, sizeof(GLint));



  mDiffuseTexture = NULL;
  mNormalTexture = NULL;
}

void Material::Bind()
{
  mDiffuseReflectance.Bind();
  mSpecularReflectance.Bind();
  mShininess.Bind();
  mHasDiffuseTexture.Bind();
  mHasNormalTexture.Bind();
  mAmbient.Bind();
  mDiffuseTextureUnit.Bind();
  mNormalTextureUnit.Bind();

  if(mDiffuseTexture != NULL)
  {
    mDiffuseTexture->Bind();
  }

  if(mNormalTexture != NULL)
  {
    mNormalTexture->Bind();
  }

}

void Material::UnBind()
{

}

void Material::SetDiffuseReflectance(glm::vec3 reflectance)
{
  mDiffuseReflectance.SetData(glm::value_ptr(reflectance), sizeof(glm::vec3));
}

void Material::SetSpecularReflectance(glm::vec3 reflectance)
{
  mSpecularReflectance.SetData(glm::value_ptr(reflectance), sizeof(glm::vec3));
}

void Material::SetShininess(float shininess)
{
  mShininess.SetData(&shininess, sizeof(GLfloat));
}

void Material::SetDiffuseTexture(Texture *texture)
{
  mDiffuseTexture = texture;
}

void Material::SetNormalTexture(Texture *texture)
{
  mNormalTexture = texture;
}
