#ifndef UTILITY_H
#define UTILITY_H

#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>


namespace RealisticEngine
{
  namespace Renderer
  {
    class Texture;
  }

  namespace Utility
  {
    /*
     * LoadTextureFromFile(std::string filename, GLenum textureUnit, GLenum format, GLenum type):
     * Parameters:
     *  1. Loads a texture from an image file.
     *  2. filename: relative path to the texture. ex: 'textures/image.bmp'
     *  3. textureUnit: the textureUnit to bind to during draw calls
     *  4. format: pixle format of the data.  ex: GL_RGB, GL_RGBA
     *  5. type: data type of pixle. ex: GL_UNSIGNED_BYTE, GL_FLOAT
     */
    Renderer::Texture* LoadTextureFromFile(std::string filename, GLenum textureUnit = GL_TEXTURE0, GLenum type=GL_UNSIGNED_BYTE);
  }
}

#endif // UTILITY_H
