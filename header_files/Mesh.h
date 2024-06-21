#ifndef MESH_H
#define MESH_H
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <iostream>
#include "Shader.h"
#include "Material.h"
namespace BERender 
{
  struct Tuple3
  {
    int a, b, c;
  };

  class Mesh
  {
  public:
    std::vector<unsigned int> indices;
    Material material;
    Mesh() {  }
    void draw(Shader& shader, Material* material);
    void draw(Shader& shader);
    void setup();
    unsigned int EBO;
  };
};
#endif