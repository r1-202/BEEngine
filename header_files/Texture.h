#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>
#include <vector>
namespace BERender
{
  struct Texture
  {
    unsigned int id;
    std::string name;
    int width;
    int height;
    int nr_components;
    void load(std::string path);
    unsigned int loadTexture(const std::string location);
    unsigned int loadCubemap(std::vector<std::string> faces);
  };
};
#endif
