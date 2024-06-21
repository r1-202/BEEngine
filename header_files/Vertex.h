#ifndef VERTEX_H
#define VERTEX_H
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
namespace BERender
{
  struct Vertex
  {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coordinates;
    glm::vec3 tangent;
    glm::vec3 bitangent;
  };
};
#endif