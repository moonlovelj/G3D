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
    std::string inputfile = "\\..\\..\\resource\\test.obj";
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

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++)
    {
      // Loop over faces(polygon)
      size_t index_offset = 0;
      TriangleMesh* pMesh = new TriangleMesh();
      for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
      {
        int fv = shapes[s].mesh.num_face_vertices[f];
        Triangle triangle;
        // Loop over vertices in the face.
        for (size_t v = 0; v < fv; v++) {
          // access to vertex
          tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
          tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
          tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
          tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
          tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
          tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
          tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
          Vertex vertex;
          vertex.position = Vector3D(vx, vy, vz);
          triangle.vertices[v] = vertex;
          //tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
          //tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
          // Optional: vertex colors
          // tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
          // tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
          // tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
        }
        index_offset += fv;
        pMesh->triangleList.push_back(triangle);
        // per-face material
        shapes[s].mesh.material_ids[f];
      }
      sceneObjects.insert(std::make_pair(pMesh->Index(), pMesh));
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