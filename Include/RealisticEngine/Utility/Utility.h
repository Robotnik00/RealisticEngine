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

    class Sphere
    {
    public:
      uint32_t* indices;
      float* positions;
      float* normals;
      uint32_t numPos;
      uint32_t numIndices;
    };

    /*
     * Create a sphere with radius r
     */
    Sphere CreateSphere(float r, int hsects, int vsects);
  }
}

#endif // UTILITY_H
