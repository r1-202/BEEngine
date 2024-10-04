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

void Model::setShader(Shader *shader)
{
  for (int i = 0; i < meshes.size(); ++i)
  {
    meshes[i].shader = shader;
  }
}

void Model::setModelMatrix(glm::mat4 model)
{
  for (int i = 0; i < meshes.size(); ++i)
  {
    meshes[i].model = model;
  }
}

void Model::loadOBJ(std::string path)
{
  Parser::splitTail(path, obj_file, directory);
  std::fstream obj_file_stream;
  obj_file_stream.open(path);
  if (!obj_file_stream.is_open())
  {
    std::cout << path << " file not open successfully";
  }
  std::string buffer = "";

  std::vector<glm::vec3> positions;
  positions.push_back(glm::vec3(0.));

  std::vector<glm::vec3> normals;
  normals.push_back(glm::vec3(0.));

  std::vector<glm::vec2> texture_coordinates;
  texture_coordinates.push_back(glm::vec2(0.));

  std::vector<int> indices;
  indices.push_back(-1);

  while (getline(obj_file_stream, buffer))
  {
    int i = 0;
    std::string token = Parser::getNextToken(buffer, i);
    if (token == "v")
    {
      positions.emplace_back(Parser::getNextFloat(buffer, i),
                             Parser::getNextFloat(buffer, i),
                             Parser::getNextFloat(buffer, i));
      indices.emplace_back(-1);
    }
    else if (token == "vn")
    {
      normals.emplace_back(Parser::getNextFloat(buffer, i),
                           Parser::getNextFloat(buffer, i),
                           Parser::getNextFloat(buffer, i));
    }
    else if (token == "vt")
    {
      texture_coordinates.emplace_back(Parser::getNextFloat(buffer, i),
                                       Parser::getNextFloat(buffer, i));
    }
    else if (token == "f")
    {
      for (int j = 0; j < 3; ++j)
      {
        Parser::Triple triple = Parser::getNextTriple(buffer, i);
        if (indices[triple.a] == -1)
        {
          indices[triple.a] = meshes.back().geometry->getVertexCount();
          meshes.back().geometry->addVertex(positions[triple.a],
                                            normals[triple.b],
                                            texture_coordinates[triple.c]);
        }
        meshes.back().geometry->indices.emplace_back(indices[triple.a]);
      }
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
      meshes.emplace_back();
      meshes.back().geometry = new Geometry();
      meshes.back().material = material;
    }
  }
}