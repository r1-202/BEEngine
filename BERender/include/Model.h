#ifndef MODEL_H
#define MODEL_H

#include <Mesh.h>
#include <vector>

namespace BERender
{
  class Model
  {
  public:
    std::string directory;
    std::string obj_file;
    std::string mtl_file;
    std::vector<Mesh> meshes;

    void draw();
    void load(std::string path);
    void loadOBJ(std::string path);
    void setShader(Shader *shader);
    void setModelMatrix(glm::mat4 model);
  };
}
#endif // MODEL_H