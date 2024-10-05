#include <Material.h>
#include <Parser.h>

using namespace BERender;

void Material::load(std::string path, std::string name, std::string directory)
{
  // defaults
  this->name = name;
  ambient_color = glm::vec3(1.0, 1.0, 1.0);
  diffuse_color = glm::vec3(1.0, 1.0, 1.0);
  specular_color = glm::vec3(1.0, 1.0, 1.0);
  emission_color = glm::vec3(1.0, 1.0, 1.0);
  alpha = 1.0;
  specular_exponent = 250;
  index_of_refraction = 1.0;

  std::fstream mtl_file_stream;
  mtl_file_stream.open(path);
  if (!mtl_file_stream.is_open())
  {
    std::cout << path << " file not opened successfully\n";
  }
  std::string buffer;
  bool found = false;

  while (getline(mtl_file_stream, buffer) && !found)
  {
    int i = 0;
    std::string token = Parser::getNextToken(buffer, i);
    if (token == "newmtl")
    {
      token = Parser::getNextToken(buffer, i);
      if (token == name)
      {
        found = true;
      }
    }
  }

  while (found && getline(mtl_file_stream, buffer))
  {
    int i = 0;
    std::string token = Parser::getNextToken(buffer, i);
    if (token == "Ka")
    {
      float x = Parser::getNextFloat(buffer, i);
      float y = Parser::getNextFloat(buffer, i);
      float z = Parser::getNextFloat(buffer, i);
      ambient_color = glm::vec3(x, y, z);
    }
    else if (token == "Kd")
    {
      float x = Parser::getNextFloat(buffer, i);
      float y = Parser::getNextFloat(buffer, i);
      float z = Parser::getNextFloat(buffer, i);
      diffuse_color = glm::vec3(x, y, z);
    }
    else if (token == "Ks")
    {
      float x = Parser::getNextFloat(buffer, i);
      float y = Parser::getNextFloat(buffer, i);
      float z = Parser::getNextFloat(buffer, i);
      specular_color = glm::vec3(x, y, z);
    }
    else if (token == "Ke")
    {
      float x = Parser::getNextFloat(buffer, i);
      float y = Parser::getNextFloat(buffer, i);
      float z = Parser::getNextFloat(buffer, i);
      emission_color = glm::vec3(x, y, z);
    }
    else if (token == "d")
    {
      alpha = Parser::getNextFloat(buffer, i);
    }
    else if (token == "Ns")
    {
      specular_exponent = Parser::getNextFloat(buffer, i);
    }
    else if (token == "Ni")
    {
      index_of_refraction = Parser::getNextFloat(buffer, i);
    }
    else if (token == "Pr")
    {
      roughness = Parser::getNextFloat(buffer, i);
    }
    else if (token == "map_Kd")
    {
      diffuse_map.load(directory + "/" + Parser::getNextToken(buffer, i));
    }
    else if (token == "map_Ks")
    {
      specular_map.load(directory + "/" + Parser::getNextToken(buffer, i));
    }
    else if (token == "map_Ke")
    {
      emission_map.load(directory + "/" + Parser::getNextToken(buffer, i));
    }
    else if (token == "map_Bump")
    {
      normal_map.load(directory + "/" + Parser::getNextToken(buffer, i));
    }
    else if (token == "map_Pr")
    {
      roughness_map.load(directory + "/" + Parser::getNextToken(buffer, i));
    }
    else if (token == "map_Pa")
    {
      ambient_occlusion_map.load(directory + "/" + Parser::getNextToken(buffer, i));
    }
    else if (token == "newmtl")
    {
      found = false;
    }
  }
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