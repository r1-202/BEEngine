#ifndef CHARACTERS_H
#define CHARACTERS_H
#include <iostream>
#include <map>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <ft2build.h>
#include "Shader.h"
#include FT_FREETYPE_H
namespace BERender 
{
  struct Character 
  {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    long long Advance;    // Offset to advance to next glyph
  };
  struct TextRenderer
  {
    TextRenderer();
    std::map<char, Character> Characters;
    unsigned int VAO;
    unsigned int VBO;
    int loadCharacters();
    void RenderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);
  };
};
#endif