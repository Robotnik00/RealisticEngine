#include <fstream>
#include <iostream>

#include "RealisticEngine/Renderer/Shader.h"

using namespace std;
using namespace RealisticEngine::Renderer;

Shader::Shader()
{
  mActive = false;
  mCompiled = false;
  mLinked = false;
  mHasGeometryShader = false;
  mProgramID = 0;
}
Shader::~Shader()
{
  if(mLinked)
  {
    glDeleteShader(mVertexShaderObject);
    glDeleteShader(mFragmentShaderObject);
    if(mHasGeometryShader)
      glDeleteShader(mGeometryShaderObject);
  }
}

bool Shader::LoadShader(std::string vertexShader, std::string fragmentShader)
{
  ifstream vertexShaderFile, fragmentShaderFile;

  vertexShaderFile.open(vertexShader.data());
  if(!vertexShaderFile.is_open())
  {
    return false;
  }
  fragmentShaderFile.open(fragmentShader.data());
  if(!fragmentShaderFile.is_open())
  {
    return false;
  }
  string line;
  while(!vertexShaderFile.eof())
  {
    getline(vertexShaderFile, line);
    mVertexShaderSource += line + "\n";
  }
  while(!fragmentShaderFile.eof())
  {
    getline(fragmentShaderFile, line);
    mFragmentShaderSource += line + "\n";
  }
  return true;
}

bool Shader::LoadShader(std::string vertexShader, std::string fragmentShader, std::string geometryShader)
{
  Shader::LoadShader(vertexShader, fragmentShader);
  ifstream geometryShaderFile;
  geometryShaderFile.open(geometryShader.data());
  if(!geometryShaderFile.is_open())
  {
    return false;
  }
  string line;
  while(!geometryShaderFile.eof())
  {
    getline(geometryShaderFile, line);
    mGeometryShaderSource += line + "\n";
  }
  mHasGeometryShader = true;
  return true;
}

bool Shader::Compile()
{
  mVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
  mFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
  mGeometryShaderObject = glCreateShader(GL_GEOMETRY_SHADER);

  GLint vlength = mVertexShaderSource.size();
  GLint flength = mFragmentShaderSource.size();
  GLint glength = mGeometryShaderSource.size();

  GLchar* vsource = (GLchar*)mVertexShaderSource.data();
  GLchar* fsource = (GLchar*)mFragmentShaderSource.data();
  GLchar* gsource = (GLchar*)mGeometryShaderSource.data();

  glShaderSourceARB(mVertexShaderObject, 1, (const GLchar**)&vsource, &vlength);
  glShaderSourceARB(mFragmentShaderObject, 1, (const GLchar**)&fsource, &flength);
  glCompileShaderARB(mVertexShaderObject);
  glCompileShaderARB(mFragmentShaderObject);

  GLint vertCompiled, fragCompiled, geomCompiled;
  glGetObjectParameterivARB(mVertexShaderObject, GL_COMPILE_STATUS, &vertCompiled);
  glGetObjectParameterivARB(mFragmentShaderObject, GL_COMPILE_STATUS, &fragCompiled);

  if(mHasGeometryShader)
  {
    glShaderSourceARB(mGeometryShaderObject, 1, (const GLchar**)&gsource, &glength);
    glCompileShaderARB(mGeometryShaderObject);
    glGetObjectParameterivARB(mGeometryShaderObject, GL_COMPILE_STATUS, &geomCompiled);
    if(geomCompiled != GL_TRUE)
    {
      return false;
    }
  }

  if(vertCompiled != GL_TRUE || fragCompiled != GL_TRUE || (geomCompiled != GL_TRUE && mHasGeometryShader))
  {
    GLint vertMaxLogSize = 0, fragMaxLogSize = 0, geomMaxLogSize = 0;
    glGetShaderiv(mVertexShaderObject, GL_INFO_LOG_LENGTH, &vertMaxLogSize);
    glGetShaderiv(mFragmentShaderObject, GL_INFO_LOG_LENGTH, &fragMaxLogSize);

    GLchar* vertLog = new GLchar[vertMaxLogSize];
    GLchar* fragLog = new GLchar[fragMaxLogSize];
    GLchar* geomLog = new GLchar[geomMaxLogSize];
    GLsizei vertLogSize, fragLogSize, geomLogSize;

    glGetShaderInfoLog(mVertexShaderObject, vertMaxLogSize, &vertLogSize, vertLog);
    glGetShaderInfoLog(mFragmentShaderObject, fragMaxLogSize, &fragLogSize, fragLog);
    std::cout << vertLog << std::endl;
    mVertShaderLog += vertLog;
    mFragShaderLog += fragLog;
    glDeleteShader(mVertexShaderObject);
    glDeleteShader(mFragmentShaderObject);
    if(mHasGeometryShader)
    {
      glGetShaderiv(mGeometryShaderObject, GL_INFO_LOG_LENGTH, &geomMaxLogSize);
      glGetShaderInfoLog(mGeometryShaderObject, vertMaxLogSize, &geomLogSize, geomLog);
      mGeomShaderLog += geomLog;
      glDeleteShader(mGeometryShaderObject);
    }

    delete[] vertLog;
    delete[] fragLog;
    delete[] geomLog;

    return false;
  }
  mCompiled = true;

  return true;
}

bool Shader::Link()
{
  mProgramID = glCreateProgram();

  glAttachShader(mProgramID, mVertexShaderObject);
  glAttachShader(mProgramID, mFragmentShaderObject);
  if(mHasGeometryShader)
  {
    glAttachShader(mProgramID, mGeometryShaderObject);
  }

  glLinkProgram(mProgramID);

  GLint linked;
  glGetObjectParameterivARB(mProgramID, GL_LINK_STATUS, &linked);
  if(linked == GL_FALSE)
  {

    glDeleteShader(mVertexShaderObject);
    glDeleteShader(mFragmentShaderObject);
    if(mHasGeometryShader)
      glDeleteShader(mGeometryShaderObject);

    return false;
  }

  mLinked = true;

  return true;
}

bool Shader::MakeActive()
{
  if(mLinked)
  {
    glUseProgram(mProgramID);
  }
  return true;
}
