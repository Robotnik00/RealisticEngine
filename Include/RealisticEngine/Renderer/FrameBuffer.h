#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

namespace RealisticEngine
{
  namespace Renderer
  {
    class FrameBuffer
    {
    public:

      typedef enum
      {
        COLOR_BUFFER_TEXTURE,
        DEPTH_BUFFER_TEXTURE
      } BIND_OPTION;
      void Setup(int xres, int yres);
      void Initialize();
      void Bind();
      void UnBind();
      void BindTexture(BIND_OPTION, GLenum texunit);


    protected:

      GLuint mFramebufferName = 0;
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
