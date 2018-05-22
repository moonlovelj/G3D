#include "SceneObject.h"

namespace g3dcommon
{
  size_t SceneObject::kSceneObjectIndex = 0;

  SceneObject::SceneObject()
  {
    index = kSceneObjectIndex;
    ++kSceneObjectIndex;
  }

  SceneObject::~SceneObject()
  {

  }
}