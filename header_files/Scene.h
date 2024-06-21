#ifndef SCENE_H
#define SCENE_H
#include "Drawable.h"
#include "Shader.h"
namespace BERender
{
  class Scene
  {
  public:

    std::vector<Drawable*> drawables;

    void draw();
    void clear();
    void add(Drawable*);
  };
};
#endif

