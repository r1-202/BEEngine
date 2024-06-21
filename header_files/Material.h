#ifndef MATERIAL_H
#define MATERIAL_H
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Texture.h"
namespace BERender 
{
  struct Material
  {
    std::string name;
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

    void load(std::string path, std::string name, std::string directory);
  };
};
#endif