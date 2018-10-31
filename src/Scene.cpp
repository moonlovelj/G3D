#include "Scene.h"
#include "TriangleMesh.h"
#include "Light.h"

#include <iostream>

namespace g3dcommon
{
  Scene::Scene(const std::vector<std::string>& objs) :
    camera(nullptr)
  {
    for (auto name : objs)
    {
      TriangleMesh* mesh = new TriangleMesh;
      mesh->Load(name);
      sceneObjects.emplace(mesh->Index(), mesh);
    }

    SceneLight* light1 = new PointLight({ 0.5f, 0.5f, 0.5f, 1.f }, { 0, 4, -1 });
    sceneLights.push_back(light1);
    SceneLight* light2 = new AmbientLight(Color{ 0.4f, 0.4f, 0.4f, 1.f });
    sceneLights.push_back(light2);
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