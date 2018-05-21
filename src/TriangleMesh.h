#ifndef __TRIANGLEMESH_H__
#define __TRIANGLEMESH_H__

#include "SceneObject.h"

namespace g3dcommon
{
  /**
  * Triangle mesh for rendering triangle-based meshes in the scene.
  */
  class TriangleMesh : public SceneObject
  {
    TriangleMesh();
    ~TriangleMesh();

  };
}

#endif