#include "Scene.h"
#include "TriangleMesh.h"

#include <iostream>



namespace g3dcommon
{

  Scene::Scene():
    camera(nullptr)
  {
    TriangleMesh* mesh = new TriangleMesh;
    mesh->Load("cube.obj");
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

  void Scene::SetCamera(Camera* camera)
  {
    this->camera = camera;
    for (auto object : sceneObjects)
    {
      object.second->SetCamera(camera);
    }
  }

}