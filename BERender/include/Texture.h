#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>
#include <vector>
namespace BERender
{
  class Texture
  {
  public:
    unsigned int id;
    std::string name;
    int width;
    int height;
    int nr_components;
    void load(std::string path);
  };

  class CubeMap : Texture
  {
    public:
    void load(std::vector<std::string> faces);
  };
}

#endif // TEXTURE_H