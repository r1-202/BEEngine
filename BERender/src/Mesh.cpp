#include <Mesh.h>
using namespace BERender;

void Mesh::draw()
{
  shader->use();
  shader->setMat4("model", model);
  material->setup();
  geometry->draw();
}