#include "Scene.h"
#include "TriangleMesh.h"

namespace g3dcommon
{


  Scene::Scene()
  {
    TriangleMesh* mesh = new TriangleMesh;
    mesh->Load("");
    sceneObjects.insert(std::make_pair(mesh->Index(), mesh));
  }

  Scene::~Scene()
  {

  }

  void Scene::Render(Renderer* renderer)
  {
    if (nullptr == renderer)
    {
      return;
    }

    for (auto object : sceneObjects)
    {
      object.second->Render(renderer);
    }
  }

}