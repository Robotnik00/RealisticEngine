#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

namespace RealisticEngine
{
  namespace Renderer
  {
    /*
     * encapsulates code for renderering depth and color
     * buffers to a frame buffer and binding them to
     * textures for another shader pass.
     */
    class FrameBuffer
    {
    public:

      typedef enum
      {
        COLOR_BUFFER_TEXTURE,
        DEPTH_BUFFER_TEXTURE
      } BIND_OPTION;

      FrameBuffer() { mFramebufferName = 0; }

      void Setup(int xres, int yres);
      void Initialize();
      void Bind();
      void UnBind();
      void BindTexture(BIND_OPTION, GLenum texunit);


    protected:

      GLuint mFramebufferName;
      // The texture we're going to render to
      GLuint mRenderedTexture;
      // The texture we're going to render to
      GLuint mDepthrenderTexture;
      // The depth buffer
      GLuint mDepthrenderbuffer;

      GLint mXRES;
      GLint mYRES;
    };
  }
}

#endif // FRAMEBUFFER_H
