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

  class Vertextn : public Vertex
  {
  public:
    glm::vec3 tangent;
  };

  class Vertextnbtn : public Vertextn
  {
  public:
    glm::vec3 bitangent;
  };

  class Vertexpn
  {
  public:
    glm::vec3 position;
    glm::vec3 normal;
  };

}

#endif // VERTEX_H