#include "Drawable.h"
using namespace BERender;
void DrawableModel::draw()
{
  shader.use();
  shader.setMat4("model", model_matrix);
  model->draw(shader);
}
DrawableGeometry::DrawableGeometry(Geometry* geometry, glm::mat4 model_matrix, Shader shader)
{
  this->geometry = geometry;
  this->model_matrix = model_matrix;
  this->shader = shader;
  this->material = &geometry->material;
}
DrawableGeometry::DrawableGeometry(Geometry* geometry, Material* material, glm::mat4 model_matrix, Shader shader)
{
  this->geometry = geometry;
  this->model_matrix = model_matrix;
  this->shader = shader;
  this->material = material;
}
void DrawableGeometry::draw()
{
  shader.use();
  shader.setMat4("model", model_matrix);
  geometry->draw(shader, material);
}