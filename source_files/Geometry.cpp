#include "Geometry.h"
#include <map>
using namespace BERender;
Geometry::Geometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
  this->vertices = vertices;
  this->indices = indices;
  setup();
  material.diffuse_color = glm::vec3(0.4, 0.4, 0.4);
}
void Geometry::setup()
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, (vertices.size()) * sizeof(Vertex), &(vertices)[0], GL_STATIC_DRAW);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indices.size()) * sizeof(unsigned int), &(indices)[0], GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

  glBindVertexArray(0);
}
void Geometry::draw(Shader shader)
{
  shader.use();
  glBindVertexArray(VAO);
  shader.setVec3("color", material.diffuse_color);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
void Geometry::draw(Shader shader, Material* material)
{
  shader.use();
  glBindVertexArray(VAO);
  shader.setVec3("color", material->diffuse_color);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
void Geometry::makeSphere()
{
  vertices.clear();
  indices.clear();
  material.diffuse_color = glm::vec3(0.4, 0., 0.);
  const unsigned int X_SEGMENTS = 64;
  const unsigned int Y_SEGMENTS = 64;
  const float PI = 3.14159265359f;
  for (unsigned int x = 0; x < X_SEGMENTS; ++x)
  {
    for (unsigned int y = 0; y < Y_SEGMENTS + 1; ++y)
    {
      float x_segment = (float)x / (float)X_SEGMENTS;
      float y_segment = (float)y / (float)Y_SEGMENTS;

      float x_position = std::cos(x_segment * 2.0f * PI) * std::sin(y_segment * PI);
      float y_position = std::cos(y_segment * PI);
      float z_position = std::sin(x_segment * 2.0f * PI) * std::sin(y_segment * PI);
      Vertex v;
      v.position = glm::vec3(x_position, y_position, z_position);
      v.normal = glm::vec3(x_position, y_position, z_position);
      vertices.push_back(v);
    }
  }

  for (unsigned int x = 0; x < X_SEGMENTS - 1; ++x)
  {
    for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
    {
      unsigned int current = x * (Y_SEGMENTS + 1) + y;
      indices.push_back(current);
      indices.push_back(current + (Y_SEGMENTS + 1));
      indices.push_back(current + (Y_SEGMENTS + 1) + 1);
      indices.push_back(current);
      indices.push_back(current + (Y_SEGMENTS + 1) + 1);
      indices.push_back(current + 1);
    }
  }
  for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
  {
    unsigned int current = (X_SEGMENTS - 1) * (Y_SEGMENTS + 1) + y;
    indices.push_back(current);
    indices.push_back(y);
    indices.push_back(y + 1);
    indices.push_back(current);
    indices.push_back(y + 1);
    indices.push_back(current + 1);
  }
  setup();
}
void Geometry::makeUVSphere()
{
  vertices.clear();
  indices.clear();
  material.diffuse_color = glm::vec3(0.4, 0., 0.);
  const unsigned int X_SEGMENTS = 64;
  const unsigned int Y_SEGMENTS = 64;
  const float PI = 3.14159265359f;
  for (unsigned int x = 0; x < X_SEGMENTS; ++x)//size on x is X_SEGMENTS
  {
    for (unsigned int y = 0; y < Y_SEGMENTS - 1; ++y)//size on y is Y_SEGMENTS-1
    {
      float x_segment = (float)x / (float)X_SEGMENTS;
      float y_segment = (float)(y + 1) / (float)Y_SEGMENTS;

      float x_position = std::cos(x_segment * 2.0f * PI) * std::sin(y_segment * PI);
      float y_position = std::cos(y_segment * PI);
      float z_position = std::sin(x_segment * 2.0f * PI) * std::sin(y_segment * PI);
      Vertex v;
      v.position = glm::vec3(x_position, y_position, z_position);
      v.normal = glm::vec3(x_position, y_position, z_position);
      vertices.push_back(v);
    }
  }

  for (unsigned int x = 0; x < X_SEGMENTS - 1; ++x)
  {
    for (unsigned int y = 0; y < Y_SEGMENTS - 2; ++y)
    {
      unsigned int current = x * (Y_SEGMENTS - 1) + y;
      indices.push_back(current);
      indices.push_back(current + Y_SEGMENTS - 1);
      indices.push_back(current + Y_SEGMENTS - 1 + 1);
      indices.push_back(current);
      indices.push_back(current + Y_SEGMENTS - 1 + 1);
      indices.push_back(current + 1);
    }
  }
  for (unsigned int y = 0; y < Y_SEGMENTS - 2; ++y)
  {
    unsigned int current = (X_SEGMENTS - 1) * (Y_SEGMENTS - 1) + y;
    indices.push_back(current);
    indices.push_back(y);
    indices.push_back(y + 1);
    indices.push_back(current);
    indices.push_back(y + 1);
    indices.push_back(current + 1);
  }
  Vertex v;
  v.position = glm::vec3(0, 1, 0);
  v.normal = v.position;
  vertices.push_back(v);
  v.position = glm::vec3(0, -1, 0);
  v.normal = v.position;
  vertices.push_back(v);
  //top
  for (unsigned int x = 0; x < X_SEGMENTS - 1; ++x)
  {
    indices.push_back(vertices.size() - 2);
    indices.push_back((x + 1) * (Y_SEGMENTS - 1));
    indices.push_back((x) * (Y_SEGMENTS - 1));
  }
  indices.push_back(vertices.size() - 2);
  indices.push_back(0);
  indices.push_back((X_SEGMENTS - 1) * (Y_SEGMENTS - 1));
  //bottom
  for (unsigned int x = 0; x < X_SEGMENTS - 1; ++x)
  {
    indices.push_back(vertices.size() - 1);
    indices.push_back((x + 1) * (Y_SEGMENTS - 1) - 1);
    indices.push_back((x + 1 + 1) * (Y_SEGMENTS - 1) - 1);
  }
  indices.push_back(vertices.size() - 1);
  indices.push_back((X_SEGMENTS) * (Y_SEGMENTS - 1) - 1);
  indices.push_back(Y_SEGMENTS - 1 - 1);
  setup();
}

void Geometry::makeIcoSphere()
{
  material.diffuse_color = glm::vec3(1, 0, 0);
  void (*subdivide)(std::vector<Vertex> &vertices, std::vector<unsigned int>&indices) =
    [](std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
  {
    std::vector<unsigned int> result;
    std::map<std::pair<unsigned int, unsigned int>, unsigned int> lookup;
    for (unsigned int i = 0; i < indices.size(); i += 3)
    {
      unsigned int mid[3];
      for (unsigned int edge = 0; edge < 3; ++edge)
      {
        unsigned int first = indices[i + edge];
        unsigned int second = indices[i + ((edge + 1) % 3)];
        std::pair<unsigned int, unsigned int> key(first, second);
        if (key.first > key.second)
          std::swap(key.first, key.second);
        std::pair<std::map<std::pair<unsigned int, unsigned int>, unsigned int>::iterator, bool> inserted = 
         lookup.insert({ key ,vertices.size() });
        if (inserted.second)
        {
          glm::vec3 edge0 = vertices[first].position;
          glm::vec3 edge1 = vertices[second].position;
          Vertex v;
          v.position = glm::normalize(edge0 + edge1);
          v.normal = v.position;
          vertices.push_back(v);
        }
        mid[edge] = inserted.first->second;
      }
      result.push_back(indices[i]);
      result.push_back(mid[0]);
      result.push_back(mid[2]);

      result.push_back(indices[i + 1]);
      result.push_back(mid[1]);
      result.push_back(mid[0]);

      result.push_back(indices[i + 2]);
      result.push_back(mid[2]);
      result.push_back(mid[1]);

      result.push_back(mid[0]);
      result.push_back(mid[1]);
      result.push_back(mid[2]);
    }
    indices = result;
  };
  unsigned int subdivisions = 5;
  const float X = .525731112119133606f;
  const float Z = .850650808352039932f;
  const float N = 0.f;

  float positions[][3] =
  {
    {-X,N,Z}, {X,N,Z}, {-X,N,-Z}, {X,N,-Z},
    {N,Z,X}, {N,Z,-X}, {N,-Z,X}, {N,-Z,-X},
    {Z,X,N}, {-Z,X, N}, {Z,-X,N}, {-Z,-X, N}
  };

  for (unsigned int i = 0; i < 12; i++)
  {
    Vertex v;
    v.position = glm::vec3(positions[i][0], positions[i][1], positions[i][2]);
    v.normal = v.position;
    vertices.push_back(v);
  }

  unsigned int triangles[][3] =
  {
    {0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
    {8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
    {7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
    {6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
  };

  for (unsigned int i = 0; i < 20; i++)
  {
    indices.push_back(triangles[i][0]);
    indices.push_back(triangles[i][2]);
    indices.push_back(triangles[i][1]);
  }
  for (unsigned int i = 0; i < subdivisions; ++i)
    subdivide(vertices, indices);
  setup();
}

void Geometry::makeCube()
{
  float source[] = {
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,//0 // Bottom-left
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,//1 // top-right
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,//2 // bottom-right         
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,//3 // top-right
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,//4 // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,//5 // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,//6 // bottom-left
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,//7 // bottom-right
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,//8 // top-right
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,//9 // top-right
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,//10 // top-left
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,//11 // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,//12 // top-right
    -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,//13 // top-left
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,//14 // bottom-left
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,//15 // bottom-left
    -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,//16 // bottom-right
    -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,//17 // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,//18 // top-left
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,//19 // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,//20 // top-right         
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,//21 // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,//22 // top-left
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,//23 // bottom-left     
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,//24// top-right
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,//25// top-left
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,//26// bottom-left
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,//27// bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,//28// bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,//29// top-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,//30 // top-left
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,//31 // bottom-right
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,//32 // top-right     
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,//33 // bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,//34 // top-left
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,//35 // bottom-left        
  };
  vertices.clear();
  indices.clear();
  material.diffuse_color = glm::vec3(0., 0.4, 0.);
  for (unsigned int i = 0; i < 36; i++)
  {
    Vertex v;
    v.position.x = source[i * 6];
    v.position.y = source[i * 6 + 1];
    v.position.z = source[i * 6 + 2];
    v.normal.x = source[i * 6 + 3];
    v.normal.y = source[i * 6 + 4];
    v.normal.z = source[i * 6 + 5];
    vertices.push_back(v);
    indices.push_back(i);
  }
  setup();
}

void Geometry::makePlane()
{
  float source[] = {
     5.0f, 0.f,  5.0f,  0.0, 1.0, 0.0,//0
    -5.0f, 0.f, -5.0f,  0.0, 1.0, 0.0,//2
    -5.0f, 0.f,  5.0f,  0.0, 1.0, 0.0,//1
              
     5.0f, 0.f,  5.0f,  0.0, 1.0, 0.0,//3
     5.0f, 0.f, -5.0f,  0.0, 1.0, 0.0,//5
    -5.0f, 0.f, -5.0f,  0.0, 1.0, 0.0,//4
  };
  vertices.clear();
  indices.clear();
  material.diffuse_color = glm::vec3(0., 0., 0.4);
  for (unsigned int i = 0; i < 6; i++)
  {
    Vertex v;
    v.position.x = source[i * 6];
    v.position.y = source[i * 6 + 1];
    v.position.z = source[i * 6 + 2];
    v.normal.x = source[i * 6 + 3];
    v.normal.y = source[i * 6 + 4];
    v.normal.z = source[i * 6 + 5];
    vertices.push_back(v);
    indices.push_back(i);
  }
  setup();
}

SoftGeometry::SoftGeometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
  this->vertices = vertices;
  this->indices = indices;
  setup();
  updateNormals();
  material.diffuse_color = glm::vec3(0.4, 0.4, 0.4);
}

void SoftGeometry::setup()
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, (vertices.size()) * sizeof(Vertex), &(vertices)[0], GL_DYNAMIC_DRAW);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indices.size()) * sizeof(unsigned int), &(indices)[0], GL_DYNAMIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

  glBindVertexArray(0);
}

void SoftGeometry::updateNormals()
{
  for (unsigned int i = 0; i < vertices.size(); i++)
  {
    vertices[i].normal = glm::vec3(0, 0, 0);
  }
  for (unsigned int i = 0; i < indices.size() - 2; i = i + 3)
  {
    Vertex* v0 = &vertices[indices[i]];
    Vertex* v1 = &vertices[indices[i + 1]];
    Vertex* v2 = &vertices[indices[i + 2]];

    glm::vec3 edge1 = v1->position - v0->position;
    glm::vec3 edge2 = v2->position - v0->position;

    glm::vec3 normal = glm::cross(edge1, edge2);
    v0->normal += normal;
    v1->normal += normal;
    v2->normal += normal;
  }
  for (int i = 0; i < vertices.size(); i++)
  {
    vertices[i].normal = glm::normalize(vertices[i].normal);
  }
}

void SoftGeometry::draw(Shader shader)
{
  updateNormals();
  shader.use();
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertices.size(), &(vertices[0]));
  shader.setVec3("color", material.diffuse_color);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
void SoftGeometry::draw(Shader shader, Material* material)
{
  updateNormals();
  shader.use();
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, (vertices.size()) * sizeof(Vertex), &(vertices)[0], GL_DYNAMIC_DRAW);
  shader.setVec3("color", material->diffuse_color);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

float SoftGeometry::calculateVolume()
{
  return 0;
}