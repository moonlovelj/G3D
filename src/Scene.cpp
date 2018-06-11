#include "Scene.h"
#include "TriangleMesh.h"
#include "Light.h"

#include <iostream>

namespace g3dcommon
{

  Scene::Scene() :
    camera(nullptr)
  {
    TriangleMesh* mesh = new TriangleMesh;
    mesh->Load("cube.obj");
    sceneObjects.insert(std::make_pair(mesh->Index(), mesh));

    auto light1 = new PointLight({ 0.1f, 0.1f, 0.1f, 1.f }, { 0, 4, -1 });
    sceneLights.push_back(light1);
    auto light = new DirectionalLight({ 0.6f, 0.6f, 0.6f, 1.f }, { -1,-1,0 });
    sceneLights.push_back(light);
    auto light12 = new AmbientLight(Color{ 0.2f, 0.2f, 0.2f, 1.f });
    sceneLights.push_back(light12);
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