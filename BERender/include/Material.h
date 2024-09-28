#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <Texture.h>
#include <glm/glm.hpp>

namespace BERender
{
  class MaterialTemplate
  {
  public:
    std::string name;
    virtual void load(std::string path, std::string name, std::string directory) = 0;
    virtual void setup() = 0;
  };

  class Material : MaterialTemplate
  {
  public:
    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    glm::vec3 emission_color;
    float alpha;
    float specular_exponent;
    float index_of_refraction;
    float roughness;
    Texture diffuse_map;
    Texture specular_map;
    Texture emission_map;
    Texture normal_map;
    Texture roughness_map;
    Texture ambient_occlusion_map;

    virtual void load(std::string path, std::string name, std::string directory);
    virtual void setup();
  };

  class MaterialUniform : MaterialTemplate
  {
  public:
    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    glm::vec3 emission_color;
    float alpha;
    float specular_exponent;
    float index_of_refraction;
    float roughness;

    virtual void load(std::string path, std::string name, std::string directory);
    virtual void setup();

    MaterialUniform(Material &material);
  };
}

#endif // MATERIAL_H