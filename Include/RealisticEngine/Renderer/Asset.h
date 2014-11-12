#ifndef ASSET_H
#define ASSET_H

#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>



namespace RealisticEngine
{
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

      UniformVariable() {}
      UniformVariable(std::string name, UniformType type, GLsizei count);

      void Setup(std::string name, UniformType type, GLsizei count);

      virtual void Bind();
      virtual void UnBind() {}

      void BindData(void* data);
      void BindActiveShader(GPURenderer* activeShaderId);

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

  }
}

#endif // ASSET_H
