#ifndef __SCENEOBJECT_H__
#define __SCENEOBJECT_H__

#include "Vector3D.h"

namespace g3dcommon
{
  /**
  * Vertex data structure.
  */
  struct Vertex
  {
    // Position of vertex.
    Vector3D position;
  };

  /**
  * Triangle data structure.
  */
  struct Triangle
  {
    // Three vertices of triangle.
    Vertex vertices[3];
  };

  /**
  * The base class of objects in the scene.
  */
  class SceneObject
  {
  public:
    SceneObject();
    virtual ~SceneObject();

    // Scene object rendering function.
    virtual void Render() = 0;

   protected:
     bool isVisible;
     // World-space position, rotation, scale.
     Vector3D position;
     Vector3D rotation;
     Vector3D scale;
  };
}

#endif