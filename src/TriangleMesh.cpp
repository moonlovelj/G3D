#include "TriangleMesh.h"
#include "SoftwareRenderer.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

#include <direct.h>
#include <iostream>

namespace g3dcommon
{


  TriangleMesh::TriangleMesh()
  {

  }

  TriangleMesh::~TriangleMesh()
  {

  }

  void TriangleMesh::Load(const std::string& objName)
  {
    std::string inputfile = "\\..\\..\\resource\\test.obj";
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    char buff[1000];
    _getcwd(buff, 1000);
    std::string basePath = std::string(buff) + "\\..\\..\\resource\\";
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, (basePath + objName).c_str(), basePath.c_str());
    if (!err.empty()) 
    { // `err` may contain warning message.
      std::cerr << err << std::endl;
    }
    if (!ret)
    {
      return;
    }

    maxRadius = 0.f;
    for (size_t i = 0; i < attrib.vertices.size(); i += 3)
    {
      Vertex vertex;
      vertex.position = Vector3D(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
      vertices.push_back(vertex);
      float len = vertex.position.Norm2();
      if (len > maxRadius)
      {
        maxRadius = len;
      }
    }
    maxRadius = sqrt(maxRadius);

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++)
    {
      // Loop over faces(polygon)
      size_t index_offset = 0;

      for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
      {
        size_t fv = shapes[s].mesh.num_face_vertices[f];
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
          indexs.push_back(idx.vertex_index);
          //tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
          //tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
          // Optional: vertex colors
          // tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
          // tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
          // tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
        }
        index_offset += fv;
        triangles.push_back(triangle);
        // per-face material
        shapes[s].mesh.material_ids[f];
      }
    }
  }

  void TriangleMesh::Render(Renderer* renderer)
  {
    if (nullptr == renderer || nullptr == camera)
    {
      return;
    }

    // Cull invisible objects based on frustum.
    if (camera->CullObject(this))
    {
      return;
    }

    const Matrix4x4& mt = GetTransformationMatrix();
    for (auto& v : vertices)
    {
      v.transformedPosition = v.position * mt;
    }

    renderer->DrawPrimitive(vertices, indexs, triangles.size(), ETriangle);
  }

}