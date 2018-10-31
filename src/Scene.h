#ifndef __SCENE_H__
#define __SCENE_H__

#include "Camera.h"
#include "Renderer.h"
#include "SceneObject.h"
#include <map>
#include <list>
#include <vector>

namespace g3dcommon
{
  class SceneLight;

  class Scene
  {
  public:
    Scene(const std::vector<std::string>& objs);
    ~Scene();

    // To render the scene.
    void Render(Renderer* renderer);
    // Set camera.
    void SetCamera(Camera* camera);
    // Returns camera.
    Camera* GetCamera() { return camera; }
    // Returns scene lights.
    const std::list<SceneLight*>& GetSceneLights() const { return sceneLights; }
  private:

    // List of objects in the scene.
    std::map<size_t, SceneObject*> sceneObjects;
    // List of lights in the scene.
    std::list<SceneLight*> sceneLights;
    // The camera used to render the scene.
    Camera* camera;
  };
}

#endif