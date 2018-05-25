#ifndef __SCENEOBJECT_H__
#define __SCENEOBJECT_H__

#include "Vector3D.h"

namespace g3dcommon
{
  class Renderer;

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