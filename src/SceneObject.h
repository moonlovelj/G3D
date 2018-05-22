#ifndef __SCENEOBJECT_H__
#define __SCENEOBJECT_H__

#include "Vector3D.h"

namespace g3dcommon
{
  class Renderer;

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
    virtual void Render(Renderer* renderer) = 0;
    // Return index of the object.
    const size_t& Index() const { return index; }
   protected:
     // Index of the object.
     static size_t kSceneObjectIndex;
     size_t index;

     // Visibility of the object.
     bool isVisible;
     // World-space position, rotation, scale.
     Vector3D position;
     Vector3D rotation;
     Vector3D scale;
  };
}

#endif