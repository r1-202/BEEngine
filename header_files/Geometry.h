#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "Vertex.h"
#include "Shader.h"
#include "Material.h"
#include <glad/glad.h>
namespace BERender 
{
  //non textured rigid body
  class Geometry
  {
  public:
    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    void setup();
    Material material;
    Geometry() {}
    Geometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void draw(Shader shader);
    void draw(Shader shader, Material* material);
    void makeSphere();
    void makeUVSphere();
    void makeCube();
    void makePlane();
    void makeIcoSphere();
  };

  class SoftGeometry: public Geometry
  {
  public:
    void setup();
    SoftGeometry() {}
    SoftGeometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void draw(Shader shader);
    void draw(Shader shader, Material* material);
    void updateNormals();
    float calculateVolume();
  };
};

#endif