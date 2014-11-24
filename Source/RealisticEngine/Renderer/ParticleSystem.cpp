#include <iostream>
#include <algorithm>

#include "RealisticEngine/Renderer/ParticleSystem.h"

using namespace RealisticEngine::Renderer;
using namespace RealisticEngine::Scene;


void ParticleSystem::Setup(Camera *camera, GPURenderer* renderer)
{
  mCamera = camera;
  mRenderer = renderer;
}

bool compare(glm::vec3 lhs, glm::vec3 rhs)
{
  if(lhs.y > rhs.y)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void ParticleSystem::SetParticles(GLfloat *particleposbuffer, int numParticles)
{
  mNumParticles = numParticles;

//  std::vector<glm::vec3> particles;
//  particles.assign((glm::vec3*)particleposbuffer, (glm::vec3*)particleposbuffer + numParticles);

//  std::sort(particles.begin(), particles.end(), compare);

//  mQuadPosition.Update(particles.data(), mNumParticles);
  mQuadPosition.Update(particleposbuffer, mNumParticles);
}

void ParticleSystem::Initialize()
{

  sphere = RealisticEngine::Utility::CreateSphere(.3, 10,10);




  if(mShader.LoadShader("particle.vert", "particle.frag") == false)
  {
    std::cout << "cant load shader\n";
  }
  if(mShader.Compile() == false)
  {
    std::cout << "cant compile shader\n";

    std::cout << mShader.GetVertexShaderLog() << std::endl;
    std::cout << mShader.GetFragmentShaderLog() << std::endl;

  }
  if(mShader.Link() == false)
  {
    std::cout << "cant link shader\n";

  }

  std::cout << "particle engine initialized\n";

  std::cout.flush();

  mQuadBuffer.Setup(mRenderer, "position", sphere.positions, GL_FLOAT, 12, sphere.numPos, 3);
  mQuadBuffer.Load();

  mQuadIndices.Setup(mRenderer, sphere.indices, sphere.numIndices);
  mQuadIndices.Load();

  mQuadPosition.Setup(mRenderer, "positionOffset", NULL, GL_FLOAT, 12, 0, 3);
  mQuadPosition.Load();

  mNumParticles = 0;

}



void ParticleSystem::Render(float delta)
{
  mRenderer->SetActiveShader(&mShader);
  mCamera->Bind();
  mQuadBuffer.Bind();
  mQuadPosition.Bind();
  mQuadIndices.Bind();
  glVertexAttribDivisor(mQuadBuffer.GetLocation(), 0);
  glVertexAttribDivisor(mQuadPosition.GetLocation(), 1);
//  glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);
  glDrawElementsInstanced(GL_TRIANGLES, sphere.numIndices, GL_UNSIGNED_INT, NULL, mNumParticles);
  glVertexAttribDivisor(0, 0);
  glVertexAttribDivisor(1, 0);
}
