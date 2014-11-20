#include <iostream>

#include "RealisticEngine/Renderer/FrameBuffer.h"

using namespace RealisticEngine::Renderer;
void FrameBuffer::Setup(int xres, int yres)
{
  mXRES = xres;
  mYRES = yres;
}

void FrameBuffer::Initialize()
{
  glGenFramebuffers(1, &mFramebufferName);
  glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferName);

  glGenTextures(1, &mRenderedTexture);
  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(GL_TEXTURE_2D, mRenderedTexture);
  // Give an empty image to OpenGL ( the last "0" )
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, mXRES, mYRES, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

  // Poor filtering. Needed !
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glGenTextures(1, &mDepthrenderTexture);
  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(GL_TEXTURE_2D, mDepthrenderTexture);
  // Give an empty image to OpenGL ( the last "0" )
  glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT32, mXRES, mYRES, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);

  // Poor filtering. Needed !
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);



  glGenRenderbuffers(1, &mDepthrenderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, mDepthrenderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mXRES, mYRES);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthrenderbuffer);

  // Set "renderedTexture" as our colour attachement #0
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mRenderedTexture, 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthrenderTexture, 0);

  // Set the list of draw buffers.
  GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
  glDrawBuffers(2, DrawBuffers); // "1" is the size of DrawBuffers

  // Always check that our framebuffer is ok
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "frame buffer error\n";
}

void FrameBuffer::Bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferName);
  glViewport(0,0,mXRES, mYRES);
}

void FrameBuffer::UnBind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindTexture(BIND_OPTION option, GLenum texunit)
{

  switch(option)
  {
  case FrameBuffer::COLOR_BUFFER_TEXTURE:
    glActiveTexture(texunit);
    glBindTexture(GL_TEXTURE_2D, mRenderedTexture);
    break;
  case FrameBuffer::DEPTH_BUFFER_TEXTURE:
    glActiveTexture(texunit);
    glBindTexture(GL_TEXTURE_2D, mDepthrenderTexture);
    break;
  }
}
