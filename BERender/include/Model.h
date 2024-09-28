#ifndef MODEL_H
#define MODEL_H

#include <Mesh.h>
#include <vector>

namespace BERender
{
  class Model
  {
    std::vector<Mesh> meshes;

    void draw();
    void load(std::string path);
  };
}
#endif // MODEL_H