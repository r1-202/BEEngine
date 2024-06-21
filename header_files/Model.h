#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "Vertex.h"
namespace BERender
{
  //container class for loading .obj and .mtl
  //.obj .mtl and textures assumed to be in the same directory
  //whenever a/b/c appears, a defines b and c
  class Model
  {
  public:
    Model(std::string path);
    ~Model();
    void draw(Shader& shader);
    void draw(Shader& shader, Material* material);
    std::vector<Vertex> vertices;
    std::vector<Mesh*> meshes;
    unsigned int VAO, VBO;
    std::string directory;
    std::string obj_file;
    std::string mtl_file;
    void setup();
    void loadTangents();
  };
};
#endif