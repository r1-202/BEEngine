#ifndef MESH_H
#define MESH_H

#include <Geometry.h>
#include <Shader.h>
#include <Material.h>
#include <glm/glm.hpp>

namespace BERender
{
  class Mesh
  {
  public:
    GeometryTemplate *geometry;
    Shader *shader;
    MaterialTemplate *material;
    glm::mat4 model;

    void draw();
    void setup();
  };
}
#endif // MESH_H