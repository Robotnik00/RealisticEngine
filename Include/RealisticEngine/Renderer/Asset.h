#ifndef ASSET_H
#define ASSET_H

#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>



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
      void Setup(std::string lightName);
      virtual void Bind();
      virtual void UnBind();
    protected:
      UniformVariable mLightPostion;
    };
  }
}

#endif // ASSET_H
