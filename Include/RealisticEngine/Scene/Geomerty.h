#ifndef GEOMERTY_H
#define GEOMERTY_H

#include <GL/gl.h>
#include <vector>

namespace RealisticEngine
{
  namespace Scene
  {
    /*
     * basic struct for holding geometry needed
     * by the physics engine
     */
    class Geometry
    {
    public:
      std::vector<glm::vec3> mVertices;
      std::vector<GLuint> mIndices;
    };
  }
}

#endif // GEOMERTY_H
