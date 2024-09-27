#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

namespace BERender
{
  class Vertex
  {
  public:
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coordinates;
  };

  class Vertextnbtn: public Vertex
  {
  public:
    glm::vec3 tangent;
    glm::vec3 bitangent;
  };
}

#endif // VERTEX_H