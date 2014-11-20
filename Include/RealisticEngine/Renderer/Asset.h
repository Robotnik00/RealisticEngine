#ifndef ASSET_H
#define ASSET_H

#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <glm/glm.hpp>



namespace RealisticEngine
{
  namespace Scene
  {
    class Node;
  }

  namespace Renderer
  {
    class GPURenderer;

    class Asset
    {
    public:
      virtual void Bind() = 0;
      virtual void UnBind() = 0;
    };

    class UniformVariable : public Asset
    {
    public:


      typedef enum
      {
        INT = 0,
        INTARY,
        VEC2I,
        VEC2IARY,
        VEC3I,
        VEC3IARY,
        VEC4I,
        VEC4IARY,
        FLOAT,
        FLOATARY,
        VEC2F,
        VEC2FARY,
        VEC3F,
        VEC3FARY,
        VEC4F,
        VEC4FARY,
        MAT2F,
        MAT3F,
        MAT4F
      }UniformType;

      typedef enum
      {
        OWNER_THIS = 0,
        OWNER_EXT
      } DATA_OWNER;

      UniformVariable();
      UniformVariable(std::string name, UniformType type, GLsizei count);
      ~UniformVariable();

      void Setup(std::string name, UniformType type, GLsizei count);

      virtual void Bind();
      virtual void UnBind() {}

      void SetRenderer(GPURenderer* renderer);
      void SetData(void* data, uint16_t bytesize);



    protected:

      UniformType mType;
      void* mData;
      std::string mName;
      GLsizei mCount;
      GPURenderer* mRenderer;
    };

    class Texture : public Asset
    {
    public:
      Texture() {}
      Texture(Texture* rhs);
      void Setup(void* data, GLuint width, GLuint height, GLenum texunit, GLenum format, GLenum pixletype);
      void Load();
      void Unload();
      virtual void Bind();
      virtual void UnBind();
    protected:
      void* mData;
      GLuint mWidth, mHeight;
      GLuint mTexid;
      GLenum mTexUnit;
      GLenum mFormat;
      GLenum mPixleType;
    };

    class UpdateModelMatrix : public Asset
    {
    public:
      void Setup(Scene::Node* node, GPURenderer* renderer, std::string modlMatName, std::string normalMatName);
      virtual void Bind();
      virtual void UnBind() {}

    protected:
      Scene::Node* mNode;
      UniformVariable mModelMat;
      UniformVariable mNormalMat;
    };

    class Light : public Asset
    {
    public:
      void Setup(std::string lightName, GPURenderer* renderer);
      virtual void Bind();
      virtual void UnBind();

      void SetColor(glm::vec3 color);
      void SetPosition(glm::vec3 position);
      void SetQuadraticAttenuation(float attenuation);

      glm::vec3 GetPosition() { return mPosition; }

    protected:
      UniformVariable mLightPosition;
      UniformVariable mLightColor;
      UniformVariable mQuadraticAttenuation;
      glm::vec3 mPosition;
    };

    class Material : public Asset
    {
    public:
      void Setup(std::string materialName, GPURenderer* renderer);

      void SetDiffuseReflectance(glm::vec3 reflectance);
      void SetSpecularReflectance(glm::vec3 reflectance);
      void SetShininess(float shininess);
      void SetDiffuseTexture(Texture* texture);
      void SetNormalTexture(Texture* texture);


      virtual void Bind();
      virtual void UnBind();

    protected:

      UniformVariable mDiffuseReflectance;
      UniformVariable mSpecularReflectance;
      UniformVariable mShininess;
      UniformVariable mHasDiffuseTexture;
      UniformVariable mHasNormalTexture;
      UniformVariable mDiffuseTextureUnit;
      UniformVariable mNormalTextureUnit;
      UniformVariable mAmbient;
      Texture* mDiffuseTexture;
      Texture* mNormalTexture;


    };

  }
}

#endif // ASSET_H
