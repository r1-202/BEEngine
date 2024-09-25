#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace BERender
{
  class Shader
  {
  public:
    unsigned int ID;
    Shader();
    Shader(const char *vertex_path, const char *fragment_path);
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec2(const std::string &name, glm::vec2 value) const;
    void setVec2(const std::string &name, float x, float y) const;

  };
};

#endif // SHADER_H