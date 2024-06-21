#include "OBJLoader.h"
using namespace BERender;
void BERender::loadOBJ(std::string path, Model& model)
{
  int i;
  for (i = path.length() - 1; i > -1; i = i - 1)
  {
    if (path[i] == '/')
      break;
  }
  model.obj_file = "";
  for (int j = i + 1; j < path.length(); j++)
  {
    model.obj_file += path[j];
  }
  model.directory = "";
  for (int j = 0; j < i; j++)
  {
    model.directory += path[j];
  }
  std::fstream obj_file_stream;
  obj_file_stream.open(path);
  std::string buffer = "";
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> texture_coordinates;
  bool first = true;
  bool material_waiting_for_object = false;
  bool object_waiting_for_material = false;
  Material temp_material;
  Mesh* mesh = new Mesh();
  while (getline(obj_file_stream, buffer))
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
    if (token == "v")
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
      Vertex vertex;
      if (temp.size() > 2)
        vertex.position = glm::vec3(temp[0], temp[1], temp[2]);
      model.vertices.push_back(vertex);
    }
    else if (token == "vn")
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
        normals.push_back(glm::vec3(temp[0], temp[1], temp[2]));
      else
        normals.push_back(glm::vec3(0, 0, 0));
    }
    else if (token == "vt")
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
      if (temp.size() > 1)
      {
        texture_coordinates.push_back(glm::vec2(temp[0], temp[1]));
      }
    }
    else if (token == "f")
    {
      std::vector<Tuple3> temp;
      while (buffer[i] != '\0')
      {
        token = "";
        Tuple3 tuple;
        while (buffer[i] != '/' && buffer[i] != ' ' && buffer[i] != '\0')
        {
          token += buffer[i];
          i++;
        }
        tuple.a = stoi(token) - 1;
        i++;
        token = "";
        while (buffer[i] != '/' && buffer[i] != ' ' && buffer[i] != '\0')
        {
          token += buffer[i];
          i++;
        }
        tuple.b = stoi(token) - 1;
        i++;
        token = "";
        while (buffer[i] != '/' && buffer[i] != ' ' && buffer[i] != '\0')
        {
          token += buffer[i];
          i++;
        }
        tuple.c = stoi(token) - 1;
        while (buffer[i] == ' ' && buffer[i] != '\0')
        {
          i++;
        }
        temp.push_back(tuple);
      }
      for (int j = 2; j < temp.size(); j++)
      {
        int k = j - 1;
        mesh->indices.push_back(temp[0].a);
        mesh->indices.push_back(temp[k].a);
        mesh->indices.push_back(temp[j].a);

        //texture coords
        model.vertices[temp[0].a].texture_coordinates = texture_coordinates[temp[0].b];
        model.vertices[temp[k].a].texture_coordinates = texture_coordinates[temp[k].b];
        model.vertices[temp[j].a].texture_coordinates = texture_coordinates[temp[j].b];

        //normals
        model.vertices[temp[0].a].normal = normals[temp[0].c];
        model.vertices[temp[k].a].normal = normals[temp[k].c];
        model.vertices[temp[j].a].normal = normals[temp[j].c];
      }
    }
    else if (token == "mtllib")
    {
      //mtl file
      token = "";
      while (buffer[i] != ' ' && buffer[i] != '\0')
      {
        token += buffer[i];
        i++;
      }
      model.mtl_file = token;
    }
    else if (token == "usemtl")
    {
      //handle material
      token = "";
      while (buffer[i] != ' ' && buffer[i] != '\0')
      {
        token += buffer[i];
        i++;
      }
      int j;
      for (j = 0; j < model.meshes.size(); j++)
      {
        if (token == model.meshes[j]->material.name)
          break;
      }
      if (j == model.meshes.size())
      {
        if (object_waiting_for_material)
        {
          mesh->material.load(model.directory + "/" + model.mtl_file, token, model.directory);
          object_waiting_for_material = false;
        }
        else
        {
          temp_material.load(model.directory + "/" + model.mtl_file, token, model.directory);
          material_waiting_for_object = true;
        }
      }
      else
      {
        if (object_waiting_for_material)
        {
          mesh->material = model.meshes[j]->material;
          object_waiting_for_material = false;
        }
        else
        {
          temp_material = model.meshes[j]->material;
          material_waiting_for_object = true;
        }
      }
    }
    else if (token == "g" || token == "o")
    {
      if (first)
      {
        first = false;
        if (material_waiting_for_object)
        {
          mesh->material = temp_material;
          material_waiting_for_object = false;
        }
        else
        {
          object_waiting_for_material = true;
        }
      }
      else
      {
        model.meshes.push_back(mesh);
        mesh = new Mesh();
        if (material_waiting_for_object)
        {
          mesh->material = temp_material;
          material_waiting_for_object = false;
        }
        else
        {
          object_waiting_for_material = true;
        }
      }
    }
  }
  if (first && material_waiting_for_object)
    mesh->material = temp_material;
  model.meshes.push_back(mesh);
}