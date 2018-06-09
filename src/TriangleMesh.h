#ifndef __TRIANGLEMESH_H__
#define __TRIANGLEMESH_H__

#include "SceneObject.h"
#include "Color.h"
#include <vector>
#include <list>

namespace g3dcommon
{
  struct RenderVertex;
  struct RenderTriangle;

  struct Vertex
  {
    Vector3D posision;
    Vector3D normal;
    Color color;
  };

  struct Triangle
  {
    struct Vert 
    {
      size_t index;
      float u, v;
    };
    Vert v[3];
    int textureId;
  };

  /**
  * Triangle mesh for rendering triangle-based meshes in the scene.
  */
  class TriangleMesh : public SceneObject
  {
  public:
    TriangleMesh();
    ~TriangleMesh();

    // Load a triangle mesh from file.
    void Load(const std::string& objName);
    // TriangleMesh rendering function.
    void Render(Renderer* renderer);
    // Calculate vertex normal.
    void CalculateVertexNormals();
    // Detach all faces,create a list of render triangles.
    void DetachAllFaces();

  private:
    // Vertex vector.
    std::vector<Vertex> vertices;
    // Triangle vector.
    std::list<Triangle> triangles;
    // Vertices for rendering.
    std::vector<RenderVertex> renderVertices;
    // Triangles for rendering.
    std::list<RenderTriangle> renderTriangles;

  };
}

#endif