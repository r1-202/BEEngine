#include <Material.h>

using namespace BERender;

void Material::load(std::string path, std::string name, std::string directory)
{
}

void Material::setup()
{
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

void MaterialUniform::setup()
{
}