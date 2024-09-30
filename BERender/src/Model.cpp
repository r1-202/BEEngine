#include <Model.h>
#include <Parser.h>
#include <fstream>
#include <glm/glm.hpp>

using namespace BERender;

void Model::draw()
{
  for (int i = 0; i < meshes.size(); ++i)
  {
    meshes[i].draw();
  }
}

void Model::load(std::string path)
{
  loadOBJ(path);
}

void Model::loadOBJ(std::string path)
{
  Parser::splitTail(path, obj_file, directory);
  std::fstream obj_file_stream;
  obj_file_stream.open(path);
  std::string buffer = "";
  bool first = true;
  bool material_waiting_for_object = false;
  bool object_waiting_for_material = false;

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> texture_coordinates;

  while (getline(obj_file_stream, buffer))
  {
    int i = 0;
    std::string token = Parser::getNextToken(buffer, i);
    if (token == "v")
    {
      positions.push_back(glm::vec3(Parser::getNextFloat(buffer, i),
                                    Parser::getNextFloat(buffer, i),
                                    Parser::getNextFloat(buffer, i)));
    }
    else if (token == "vn")
    {
      normals.push_back(glm::vec3(Parser::getNextFloat(buffer, i),
                                  Parser::getNextFloat(buffer, i),
                                  Parser::getNextFloat(buffer, i)));
    }
    else if (token == "vt")
    {
      texture_coordinates.push_back(glm::vec2(Parser::getNextFloat,
                                              Parser::getNextFloat));
    }
    else if (token == "f")
    {
    }
    else if (token == "mtllib")
    {
      mtl_file = Parser::getNextToken(buffer, i);
    }
    else if (token == "usemtl")
    {
      MaterialTemplate *material;
      token = Parser::getNextToken(buffer, i);
      int j = 0;
      for (int j = 0; j < meshes.size(); ++j)
      {
        if (token == meshes[j].material->name)
          break;
      }
      if (j == meshes.size())
      {
        material = new Material();
        material->load(directory + "/" + mtl_file, token, directory);
      }
      else
      {
        material = meshes[j].material;
      }
    }
  }
}