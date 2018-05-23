#ifndef __SCENE_H__
#define __SCENE_H__

#include "Camera.h"
#include "Renderer.h"
#include "SceneObject.h"
#include <map>

namespace g3dcommon
{
 class Scene
 {
 public:
   Scene();
   ~Scene();
    
   // To render the scene.
   void Render(Renderer* renderer);

 private:

   // List of objects in the scene.
   std::map<size_t, SceneObject*> sceneObjects;
 };
}

#endif