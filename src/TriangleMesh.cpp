#include "TriangleMesh.h"
#include "SoftwareRenderer.h"
#include "Texture.h"
#include "TextureManager.h"

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
    int vetext = 0;
    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++)
    {
      // Loop over faces(polygon)
      size_t index_offset = 0;

      for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
      {
        // per-face material
        size_t texIndex = TextureManager::GetInstance().LoadTexture(basePath.c_str(), materials[shapes[s].mesh.material_ids[f]].diffuse_texname);

        size_t fv = shapes[s].mesh.num_face_vertices[f];
        Triangle triangle;
        // Loop over vertices in the face.
        for (size_t v = 0; v < fv; v++) {
          // access to vertex
          tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
          tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
          tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
          tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
//           tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
//           tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
//           tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
          Vertex vertex;
          vertex.position = Vector3D(vx, vy, vz);
          vertex.textureIndex = texIndex;

          indexs.push_back(vetext);
          ++vetext;

          tinyobj::real_t tx = 0;
          tinyobj::real_t ty = 0;
          if (idx.texcoord_index > -1)
          {
            tx = attrib.texcoords[2 * idx.texcoord_index + 0];
            ty = attrib.texcoords[2 * idx.texcoord_index + 1];
          }
          // Optional: vertex colors
          tinyobj::real_t red = attrib.colors[3 * idx.vertex_index + 0];
          tinyobj::real_t green = attrib.colors[3 * idx.vertex_index + 1];
          tinyobj::real_t blue = attrib.colors[3 * idx.vertex_index + 2];

          triangle.vertices[v] = vertex;
          vertex.color = { red, green, blue, 1.f };
          vertex.newColor = vertex.color;
          vertex.u = tx;
          vertex.v = ty;
          vertices.push_back(vertex);

          float len = vertex.position.Norm2();
          if (len > maxRadius)
          {
            maxRadius = len;
          }

        }
        index_offset += fv;
        triangles.push_back(triangle);
        ;
      }
    }
    maxRadius = sqrt(maxRadius);
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
      v.newPosition = v.position * mt;
      v.normal = { 0.f, 0.f, 0.f };
    }


    // Calculate normal of vertex.
    for (size_t i = 0; i < indexs.size(); i+=3)
    {
      Vertex& v0 = vertices[indexs[i + 0]];
      Vertex& v1 = vertices[indexs[i + 1]];
      Vertex& v2 = vertices[indexs[i + 2]];
      Vector3D faceN = Cross(v1.newPosition - v0.newPosition, v2.newPosition - v0.newPosition);
      v0.normal += faceN;
      v1.normal += faceN;
      v2.normal += faceN;
    }
    for (auto& v : vertices)
    {
      v.normal.Normalize();
    }

    renderer->DrawPrimitive(vertices, indexs, triangles.size(), ETriangle);
  }

}