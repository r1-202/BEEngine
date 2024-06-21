#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "Shader.h"
#include "Model.h"
#include <glm/glm.hpp>
#include "Geometry.h"
#include "Shader.h"
namespace BERender
{
  class Drawable
  {
  public:
    Drawable() {}
    Shader shader;
    glm::mat4 model_matrix;
    Material* material;
    virtual void draw() = 0;
  };

  class DrawableModel : public Drawable
  {
  public:
    DrawableModel() {}
    Model* model;
    virtual void draw();
  };

  class DrawableGeometry : public Drawable
  {
  public:
    Geometry* geometry;
    DrawableGeometry() {}
    DrawableGeometry(Geometry* geometry, glm::mat4 model_matrix, Shader shader);
    DrawableGeometry(Geometry* geometry, Material* material, glm::mat4 model_matrix, Shader shader);
    virtual void draw();
  };
};
#endif