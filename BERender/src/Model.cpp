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
  for (int i = 0; i < meshes.size(); ++i)
  {
    meshes[i].geometry->load();
  }
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
    std::cout << path << " file not open successfully\n";
  }
  std::string buffer = "";

  std::vector<glm::vec3> positions;

  std::vector<glm::vec3> normals;

  std::vector<glm::vec2> texture_coordinates;

  std::vector<int> indices;

  while (getline(obj_file_stream, buffer))
  {
    int i = 0;
    std::string token = Parser::getNextToken(buffer, i);
    if (token == "v")
    {
      float x = Parser::getNextFloat(buffer, i);
      float y = Parser::getNextFloat(buffer, i);
      float z = Parser::getNextFloat(buffer, i);
      positions.push_back(glm::vec3(x, y, z));
      indices.push_back(-1);
    }
    else if (token == "vn")
    {
      float x = Parser::getNextFloat(buffer, i);
      float y = Parser::getNextFloat(buffer, i);
      float z = Parser::getNextFloat(buffer, i);
      normals.push_back(glm::vec3(x, y, z));
    }
    else if (token == "vt")
    {
      float x = Parser::getNextFloat(buffer, i);
      float y = Parser::getNextFloat(buffer, i);
      texture_coordinates.push_back(glm::vec2(x, y));
    }
    else if (token == "f")
    {
      for (int j = 0; j < 3; ++j)
      {
        Parser::Triple triple = Parser::getNextTriple(buffer, i);
        --triple.a;
        --triple.b;
        --triple.c;
        if (indices[triple.a] == -1)
        {
          indices[triple.a] = meshes.back().geometry->getVertexCount();
          meshes.back().geometry->addVertex(positions[triple.a],
                                            normals[triple.b],
                                            texture_coordinates[triple.c]);
        }
        meshes.back().geometry->indices.push_back(indices[triple.a]);
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
      meshes.push_back(Mesh());
      meshes.back().geometry = new Geometry();
      meshes.back().material = material;
    }
  }
}