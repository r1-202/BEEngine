#include "Mesh.h"
using namespace BERender;
void Mesh::setup()
{
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void Mesh::draw(Shader& shader, Material* material)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  shader.use();
  shader.setVec3("material.ambient_color", material->ambient_color);
  shader.setVec3("material.diffuse_color", material->diffuse_color);
  shader.setVec3("material.specular_color", material->specular_color);
  shader.setVec3("material.emission_color", material->emission_color);
  shader.setFloat("material.alpha", material->alpha);
  shader.setFloat("material.specular_exponent", material->specular_exponent);
  shader.setFloat("material.index_of_refraction", material->index_of_refraction);
  shader.setFloat("material.roughness", material->roughness);

  glActiveTexture(GL_TEXTURE0 + 0);
  shader.setInt("material.diffuse_map", 0);
  glBindTexture(GL_TEXTURE_2D, material->diffuse_map.id);
  
  glActiveTexture(GL_TEXTURE0 + 1);
  shader.setInt("material.specular_map", 1);
  glBindTexture(GL_TEXTURE_2D, material->specular_map.id);

  glActiveTexture(GL_TEXTURE0 + 2);
  shader.setInt("material.emission_map", 2);
  glBindTexture(GL_TEXTURE_2D, material->emission_map.id);
  
  glActiveTexture(GL_TEXTURE0 + 3);
  shader.setInt("material.normal_map", 3);
  glBindTexture(GL_TEXTURE_2D, material->normal_map.id);
  
  glActiveTexture(GL_TEXTURE0 + 4);
  shader.setInt("material.roughness_map", 4);
  glBindTexture(GL_TEXTURE_2D, material->roughness_map.id);

  glActiveTexture(GL_TEXTURE0 + 5);
  shader.setInt("material.ambient_occlusion_map", 5);
  glBindTexture(GL_TEXTURE_2D, material->ambient_occlusion_map.id);

  glActiveTexture(GL_TEXTURE0);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
void Mesh::draw(Shader& shader)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  shader.use();
  shader.setVec3("material.ambient_color", material.ambient_color);
  shader.setVec3("material.diffuse_color", material.diffuse_color);
  shader.setVec3("material.specular_color", material.specular_color);
  shader.setVec3("material.emission_color", material.emission_color);
  shader.setFloat("material.alpha", material.alpha);
  shader.setFloat("material.specular_exponent", material.specular_exponent);
  shader.setFloat("material.index_of_refraction", material.index_of_refraction);
  shader.setFloat("material.roughness", material.roughness);

  glActiveTexture(GL_TEXTURE0 + 0);
  shader.setInt("material.diffuse_map", 0);
  glBindTexture(GL_TEXTURE_2D, material.diffuse_map.id);

  glActiveTexture(GL_TEXTURE0 + 1);
  shader.setInt("material.specular_map", 1);
  glBindTexture(GL_TEXTURE_2D, material.specular_map.id);

  glActiveTexture(GL_TEXTURE0 + 2);
  shader.setInt("material.emission_map", 2);
  glBindTexture(GL_TEXTURE_2D, material.emission_map.id);

  glActiveTexture(GL_TEXTURE0 + 3);
  shader.setInt("material.normal_map", 3);
  glBindTexture(GL_TEXTURE_2D, material.normal_map.id);

  glActiveTexture(GL_TEXTURE0 + 4);
  shader.setInt("material.roughness_map", 4);
  glBindTexture(GL_TEXTURE_2D, material.roughness_map.id);

  glActiveTexture(GL_TEXTURE0 + 5);
  shader.setInt("material.ambient_occlusion_map", 5);
  glBindTexture(GL_TEXTURE_2D, material.ambient_occlusion_map.id);

  glActiveTexture(GL_TEXTURE0);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}