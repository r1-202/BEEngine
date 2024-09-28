#include <Model.h>

using namespace BERender;

void Model::draw()
{
  for(int i=0;i<meshes.size();++i)
  {
    meshes[i].draw();
  }
}