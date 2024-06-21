#include "Scene.h"
using namespace BERender;
void Scene::draw()
{
  for (unsigned int i = 0; i < drawables.size(); i++)
  {
    drawables[i]->draw();
  }
}
void Scene::clear()
{
  for (unsigned int i = 0; i < drawables.size(); i++)
  {
    delete drawables[i];
  }
  drawables.clear();
}
void Scene::add(Drawable* drawable)
{
  drawables.push_back(drawable);
}