#include <Mesh.h>
using namespace BERender;

void Mesh::draw()
{
  shader->use();
  material->setup();
  geometry->draw();
}