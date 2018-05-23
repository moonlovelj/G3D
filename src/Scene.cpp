#include "Scene.h"
#include "TriangleMesh.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

#include <iostream>
#include <direct.h>


namespace g3dcommon
{


  Scene::Scene()
  {
    std::string inputfile = "\\..\\..\\resource\\cube.obj";
    std::string inputbasepath = "\\..\\..\\resource\\";
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    char buff[1000];
    _getcwd(buff, 1000);

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, (std::string(buff) + inputfile).c_str(), (std::string(buff) + inputbasepath).c_str());

    if (!err.empty()) { // `err` may contain warning message.
      std::cerr << err << std::endl;
    }

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