#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <Vertex.h>
#include <Shader.h>
#include <vector>
#include <glad/glad.h>
namespace BERender
{

  class GeometryTemplate
  {
  public:
    unsigned int VAO, VBO, EBO;
    std::vector<unsigned int> indices;
    void draw();
    virtual void load() = 0;
    virtual void makeSphere() = 0;
    virtual void makeUVSphere() = 0;
    virtual void makeCube() = 0;
    virtual void makePlane() = 0;
    virtual void makeIcoSphere() = 0;
    virtual ~GeometryTemplate() = 0;
  };

  class Geometry : public GeometryTemplate
  {
  public:
    std::vector<Vertex> vertices;
    virtual void load();
    Geometry();
    Geometry(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
    virtual void makeSphere();
    virtual void makeUVSphere();
    virtual void makeCube();
    virtual void makePlane();
    virtual void makeIcoSphere();
  };

  class Geometrypn : public GeometryTemplate
  {
  public:
    std::vector<Vertexpn> vertices;
    virtual void load();
    Geometrypn();
    Geometrypn(std::vector<Vertexpn> &vertices, std::vector<unsigned int> &indices);
    virtual void makeSphere();
    virtual void makeUVSphere();
    virtual void makeCube();
    virtual void makePlane();
    virtual void makeIcoSphere();
  };

  template <class VertexType>
  class Shaper
  {
  public:
    static void makeSphere(std::vector<VertexType> &vertices, std::vector<unsigned int> &indices);
    static void makeUVSphere(std::vector<VertexType> &vertices, std::vector<unsigned int> &indices);
    static void makeCube(std::vector<VertexType> &vertices, std::vector<unsigned int> &indices);
    static void makePlane(std::vector<VertexType> &vertices, std::vector<unsigned int> &indices);
    static void makeIcoSphere(std::vector<VertexType> &vertices, std::vector<unsigned int> &indices);
  };

};

#endif // GEOMETRY_H