#ifndef SHADER_H
#define SHADER_H

#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

namespace RealisticEngine
{
  namespace Renderer
  {
    /*
     *  code for loading a shader
     */
    class Shader
    {
    public:
      Shader();
      ~Shader();

      bool LoadShader(std::string vertexShader, std::string fragmentShader);
      bool LoadShader(std::string vertexShader, std::string framentShader, std::string geometryShader);
      bool Compile();
      bool Link();
      bool MakeActive();

      GLuint GetProgramID() { return mProgramID; }

      bool GetActive() { return mActive; }
      bool GetCompiled() { return mCompiled; }
      bool GetLinked() { return mLinked; }

      std::string GetVertexShaderLog() { return mVertShaderLog; }
      std::string GetFragmentShaderLog() { return mFragShaderLog; }
      std::string GetGeometryShaderLog() { return mGeomShaderLog; }

    protected:

      bool mCompiled;
      bool mLinked;
      bool mActive;
      bool mHasGeometryShader;


      GLuint mProgramID;
      GLuint mVertexShaderObject, mFragmentShaderObject, mGeometryShaderObject;

      std::string mVertexShaderSource, mFragmentShaderSource, mGeometryShaderSource;
      std::string mVertShaderLog, mFragShaderLog, mGeomShaderLog;
    };
  }
}


#endif // SHADER_H
