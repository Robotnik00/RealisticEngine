#include <FreeImage.h>

#include "RealisticEngine/Utility/Utility.h"

#include "RealisticEngine/Renderer/Asset.h"

using namespace RealisticEngine::Renderer;

Texture* RealisticEngine::Utility::LoadTextureFromFile(std::string filename, GLenum textureUnit, GLenum type)
{

  FIBITMAP *dib = FreeImage_Load(FreeImage_GetFileType(filename.data()), filename.data(), FreeImage_GetFileType(filename.data()));
  if(dib == NULL)
  {
    return NULL;
  }
  GLenum format = GL_RGB;
  if(FreeImage_GetChannel(dib, FICC_ALPHA) != NULL)
  {
    format = GL_RGBA;
  }

  Texture* texture = new Texture();
  texture->Setup(FreeImage_GetBits(dib), FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), textureUnit, format, type);
  texture->Load();
  FreeImage_Unload(dib);

  return texture;
}
