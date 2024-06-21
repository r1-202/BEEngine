#include "Material.h"
using namespace BERender;
void Material::load(std::string path, std::string name, std::string directory)
{
  //defaults
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
  std::string buffer;
  bool found = false;
  //find newmtl name
  while (getline(mtl_file_stream, buffer) && !found)
  {
    int i = 0;
    std::string token = "";
    while (buffer[i] == ' ' && buffer[i] != '\0')
      i++;
    while (buffer[i] != ' ' && buffer[i] != '\0')
    {
      token += buffer[i];
      i++;
    }
    while (buffer[i] == ' ' && buffer[i] != '\0')
      i++;
    if (token == "newmtl")
    {
      token = "";
      while (buffer[i] != ' ' && buffer[i] != '\0')
      {
        token += buffer[i];
        i++;
      }
      if (token == name)
      {
        found = true;
      }
    }
  }
  while (found && getline(mtl_file_stream, buffer))
  {
    int i = 0;
    std::string token = "";
    while (buffer[i] == ' ' && buffer[i] != '\0')
      i++;
    while (buffer[i] != ' ' && buffer[i] != '\0')
    {
      token += buffer[i];
      i++;
    }
    while (buffer[i] == ' ' && buffer[i] != '\0')
      i++;
    if (token == "Ka")
    {
      std::vector<float> temp;
      while (buffer[i] != '\0')
      {
        token = "";
        while (buffer[i] != ' ' && buffer[i] != '\0')
        {
          token += buffer[i];
          i++;
        }
        while (buffer[i] == ' ')
          i++;
        temp.push_back(stof(token));
      }
      if (temp.size() > 2)
        ambient_color = glm::vec3(temp[0], temp[1], temp[2]);
    }
    else if (token == "Kd")
    {
      std::vector<float> temp;
      while (buffer[i] != '\0')
      {
        token = "";
        while (buffer[i] != ' ' && buffer[i] != '\0')
        {
          token += buffer[i];
          i++;
        }
        while (buffer[i] == ' ')
          i++;
        temp.push_back(stof(token));
      }
      if (temp.size() > 2)
        diffuse_color = glm::vec3(temp[0], temp[1], temp[2]);
    }
    else if (token == "Ks")
    {
      std::vector<float> temp;
      while (buffer[i] != '\0')
      {
        token = "";
        while (buffer[i] != ' ' && buffer[i] != '\0')
        {
          token += buffer[i];
          i++;
        }
        while (buffer[i] == ' ')
          i++;
        temp.push_back(stof(token));
      }
      if (temp.size() > 2)
        specular_color = glm::vec3(temp[0], temp[1], temp[2]);
    }
    else if (token == "Ke")
    {
      std::vector<float> temp;
      while (buffer[i] != '\0')
      {
        token = "";
        while (buffer[i] != ' ' && buffer[i] != '\0')
        {
          token += buffer[i];
          i++;
        }
        while (buffer[i] == ' ')
          i++;
        temp.push_back(stof(token));
      }
      if (temp.size() > 2)
        emission_color = glm::vec3(temp[0], temp[1], temp[2]);
    }
    else if (token == "d")
    {
      token = "";
      while (buffer[i] != ' ' && buffer[i] != '\0')
      {
        token += buffer[i];
        i++;
      }
      float d = stof(token);
      alpha = d;
    }
    else if (token == "Ns")
    {
      token = "";
      while (buffer[i] != ' ' && buffer[i] != '\0')
      {
        token += buffer[i];
        i++;
      }
      float Ns = stof(token);
      specular_exponent = Ns;
    }
    else if (token == "Ni")
    {
      token = "";
      while (buffer[i] != ' ' && buffer[i] != '\0')
      {
        token += buffer[i];
        i++;
      }
      float Ni = stof(token);
      index_of_refraction = Ni;
    }
    else if (token == "Pr")
    {
    token = "";
    while (buffer[i] != ' ' && buffer[i] != '\0')
    {
      token += buffer[i];
      i++;
    }
    float r = stof(token);
    roughness = r;
    }
    else if (token == "map_Kd")
    {
      token = "";
      while (buffer[i] != ' ' && buffer[i] != '\0')
      {
        token += buffer[i];
        i++;
      }
      diffuse_map.load(directory + "/" + token);
    }
    else if (token == "map_Ks")
    {
      token = "";
      while (buffer[i] != ' ' && buffer[i] != '\0')
      {
        token += buffer[i];
        i++;
      }
      specular_map.load(directory + "/" + token);
    }
    else if (token == "map_Ke")
    {
    token = "";
    while (buffer[i] != ' ' && buffer[i] != '\0')
    {
      token += buffer[i];
      i++;
    }
    emission_map.load(directory + "/" + token);
    }
    else if (token == "map_Bump")
    {
      token = "";
      while (buffer[i] != ' ' && buffer[i] != '\0')
      {
        token += buffer[i];
        i++;
      }
      normal_map.load(directory + "/" + token);
    }
    else if (token == "map_Pr")
    {
      token = "";
      while (buffer[i] != ' ' && buffer[i] != '\0')
      {
        token += buffer[i];
        i++;
      }
      roughness_map.load(directory + "/" + token);
    }
    else if (token == "map_Pa")
    {
      token = "";
      while (buffer[i] != ' ' && buffer[i] != '\0')
      {
        token += buffer[i];
        i++;
      }
      ambient_occlusion_map.load(directory + "/" + token);
    }
    else if (token == "newmtl")
    {
      found = false;
    }
  }
}