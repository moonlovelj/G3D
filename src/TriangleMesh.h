#ifndef __TRIANGLEMESH_H__
#define __TRIANGLEMESH_H__

#include "SceneObject.h"
#include <vector>

namespace g3dcommon
{
  struct Vertex;
  struct Triangle;

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

  private:
    // Vertex vector.
    std::vector<Vertex> vertices;
    // Triangle vector.
    std::vector<Triangle> triangles;
    // Index vector.
    std::vector<size_t> indexs;
  };
}

#endif