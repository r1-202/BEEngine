#include "Model.h"
#include "OBJLoader.h"
using namespace BERender;
Model::Model(std::string path)
{
  loadOBJ(path, *this);
  loadTangents();
  setup();
}

void Model::setup()
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  for (int i = 0; i < meshes.size(); i++)
  {
    meshes[i]->setup();
  }

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinates));

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

  glBindVertexArray(0);
}
void Model::draw(Shader& shader)
{
  glBindVertexArray(VAO);
  for (int i = 0; i < meshes.size(); i++)
  {
    meshes[i]->draw(shader);
  }
  glBindVertexArray(0);
}
void Model::draw(Shader& shader, Material* material)
{
  glBindVertexArray(VAO);
  for (int i = 0; i < meshes.size(); i++)
  {
    meshes[i]->draw(shader, material);
  }
  glBindVertexArray(0);
}


void Model::loadTangents()
{
  for (unsigned int i = 0; i < vertices.size(); i++)
  {
    vertices[i].tangent = glm::vec3(0, 0, 0);
    vertices[i].bitangent = glm::vec3(0, 0, 0);
  }
  for (unsigned int m = 0; m < meshes.size(); m++)
  {
    for (unsigned int i = 0; i < meshes[m]->indices.size(); i = i + 3)
    {
      Vertex* v0 = &vertices[meshes[m]->indices[i]];
      Vertex* v1 = &vertices[meshes[m]->indices[i+1]];
      Vertex* v2 = &vertices[meshes[m]->indices[i+2]];

      glm::vec3 edge1 = v1->position - v0->position;
      glm::vec3 edge2 = v2->position - v0->position;

      float deltaU1 = v1->texture_coordinates.x - v0->texture_coordinates.x;
      float deltaV1 = v1->texture_coordinates.y - v0->texture_coordinates.y;
      float deltaU2 = v2->texture_coordinates.x - v0->texture_coordinates.x;
      float deltaV2 = v2->texture_coordinates.y - v0->texture_coordinates.y;

      float f = 1.0 / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

      glm::vec3 tangent, bitangent;

      tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
      tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
      tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

      bitangent.x = f * (-deltaU2 * edge1.x + deltaU1 * edge2.x);
      bitangent.y = f * (-deltaU2 * edge1.y + deltaU1 * edge2.y);
      bitangent.z = f * (-deltaU2 * edge1.z + deltaU1 * edge2.z);

      v0->tangent += tangent;
      v0->bitangent += bitangent;
      v1->tangent += tangent;
      v1->bitangent += bitangent;
      v2->tangent += tangent;
      v2->bitangent += bitangent;
    }
  }
  for (int i = 0; i < vertices.size(); i++)
  {
    vertices[i].tangent = glm::normalize(vertices[i].tangent);
    vertices[i].bitangent = glm::normalize(vertices[i].bitangent);
  }
}
Model::~Model()
{
  for (int i = 0; i < meshes.size(); i++)
  {
    delete meshes[i];
  }
}