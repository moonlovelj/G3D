#ifndef __TRIANGLEMESH_H__
#define __TRIANGLEMESH_H__

#include "SceneObject.h"
#include <list>

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
    void Load(const std::string& fileName);
    // TriangleMesh rendering function.
    void Render(Renderer* renderer);

 
    // Vertex list.
    std::list<Vertex> vertexList;
    // Triangle list.
    std::list<Triangle> triangleList;
  };
}

#endif