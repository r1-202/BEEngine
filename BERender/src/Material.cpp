#include <Material.h>

using namespace BERender;

void Material::load(std::string path, std::string name, std::string directory)
{
}

void Material::setup(Shader *shader)
{
  shader->use();
  shader->setVec3("material.ambient_color", ambient_color);
  shader->setVec3("material.diffuse_color", diffuse_color);
  shader->setVec3("material.specular_color", specular_color);
  shader->setVec3("material.emission_color", emission_color);
  shader->setFloat("material.alpha", alpha);
  shader->setFloat("material.specular_exponent", specular_exponent);
  shader->setFloat("material.index_of_refraction", index_of_refraction);
  shader->setFloat("material.roughness", roughness);

  glActiveTexture(GL_TEXTURE0 + 0);
  shader->setInt("material.diffuse_map", 0);
  glBindTexture(GL_TEXTURE_2D, diffuse_map.id);

  glActiveTexture(GL_TEXTURE0 + 1);
  shader->setInt("material.specular_map", 1);
  glBindTexture(GL_TEXTURE_2D, specular_map.id);

  glActiveTexture(GL_TEXTURE0 + 2);
  shader->setInt("material.emission_map", 2);
  glBindTexture(GL_TEXTURE_2D, emission_map.id);

  glActiveTexture(GL_TEXTURE0 + 3);
  shader->setInt("material.normal_map", 3);
  glBindTexture(GL_TEXTURE_2D, normal_map.id);

  glActiveTexture(GL_TEXTURE0 + 4);
  shader->setInt("material.roughness_map", 4);
  glBindTexture(GL_TEXTURE_2D, roughness_map.id);

  glActiveTexture(GL_TEXTURE0 + 5);
  shader->setInt("material.ambient_occlusion_map", 5);
  glBindTexture(GL_TEXTURE_2D, ambient_occlusion_map.id);

  glActiveTexture(GL_TEXTURE0);
}

void MaterialUniform::load(std::string path, std::string name, std::string directory)
{
  Material material;
  material.load(path, name, directory);
  *this = MaterialUniform(material);
}

MaterialUniform::MaterialUniform(Material &material)
{
  ambient_color = material.ambient_color;
  diffuse_color = material.diffuse_color;
  specular_color = material.diffuse_color;
  emission_color = material.emission_color;
  alpha = material.alpha;
  specular_exponent = material.specular_exponent;
  index_of_refraction = material.index_of_refraction;
  roughness = material.roughness;
}

void MaterialUniform::setup(Shader *shader)
{
  shader->use();
  shader->setVec3("material.ambient_color", ambient_color);
  shader->setVec3("material.diffuse_color", diffuse_color);
  shader->setVec3("material.specular_color", specular_color);
  shader->setVec3("material.emission_color", emission_color);
  shader->setFloat("material.alpha", alpha);
  shader->setFloat("material.specular_exponent", specular_exponent);
  shader->setFloat("material.index_of_refraction", index_of_refraction);
  shader->setFloat("material.roughness", roughness);
}