#include <iostream>

#include "RealisticEngine/Renderer/DrawInterface.h"
#include "RealisticEngine/Renderer/Buffer.h"

using namespace RealisticEngine::Renderer;

DrawVertexBufferObject::DrawVertexBufferObject(VertexBufferObject *vbo)
{
  mVBO = vbo;
}

void DrawVertexBufferObject::Draw()
{
  if(mVBO != NULL)
    mVBO->Render();
}



