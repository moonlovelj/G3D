#include "TriangleMesh.h"
#include "TextureManager.h"
#include "Renderer.h"
#include "Camera.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd // stupid MSFT "deprecation" warning
#else
#include <unistd.h>
#endif
#include <iostream>

namespace g3dcommon
{


    TriangleMesh::TriangleMesh()
    {

    }

    TriangleMesh::~TriangleMesh()
    {

    }

    void TriangleMesh::Load(const std::string &objName)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        char buff[1024];
        getcwd(buff, 1024);
        std::string basePath = std::string(buff) + "/../resource/";
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
        // Copy vertices.
        maxRadius = 0.f;
        vertices.resize(attrib.vertices.size() / 3);
        for (size_t i = 0; i < attrib.vertices.size(); i += 3)
        {
            vertices[i / 3].posision = {attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]};
            vertices[i / 3].color = {attrib.colors[i], attrib.colors[i + 1], attrib.colors[i + 2], 1.f};
            float len = vertices[i / 3].posision.Norm2();
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
                // per-face material
                int texIndex = TextureManager::GetInstance()
                    .LoadTexture(basePath.c_str(), materials[shapes[s].mesh.material_ids[f]].diffuse_texname);
                size_t fv = shapes[s].mesh.num_face_vertices[f];
                Triangle triangle;
                triangle.textureId = texIndex;
                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++)
                {
                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    triangle.v[v].index = idx.vertex_index;
                    if (idx.texcoord_index > -1)
                    {
                        triangle.v[v].u = attrib.texcoords[2 * idx.texcoord_index + 0];
                        triangle.v[v].v = attrib.texcoords[2 * idx.texcoord_index + 1];
                    }
                }
                triangles.push_back(triangle);
                index_offset += fv;
            }
        }

        CalculateVertexNormals();
    }

    void TriangleMesh::Render(Renderer *renderer)
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

        DetachAllFaces();

        renderer->RenderTriangleMesh(renderVertices, renderVertices.size(), renderTriangles, renderTriangles.size());
    }

    void TriangleMesh::CalculateVertexNormals()
    {
        for (auto &it : vertices)
        {
            it.normal.Zero();
        }

        for (const auto &it : triangles)
        {
            Vector3D n = Cross(vertices[it.v[1].index].posision - vertices[it.v[0].index].posision,
                               vertices[it.v[2].index].posision - vertices[it.v[0].index].posision);
            vertices[it.v[0].index].normal += n;
            vertices[it.v[1].index].normal += n;
            vertices[it.v[2].index].normal += n;
        }

        for (auto &it : vertices)
        {
            it.normal.Normalize();
        }
    }

    void TriangleMesh::DetachAllFaces()
    {
        renderTriangles.clear();
        renderVertices.resize(triangles.size() * 3);
        const Matrix4x4 &mt = GetTransformationMatrix();
        const Matrix4x4 &mtInvT = mt.Inv().T();
        size_t vIndex = 0;
        for (const auto &it : triangles)
        {
            RenderTriangle renderTriangle;
            for (size_t i = 0; i < 3; i++)
            {
                RenderVertex &v = renderVertices[vIndex];
                renderTriangle.indexs[i] = vIndex;
                v.position = vertices[it.v[i].index].posision * mt;
                v.normal = vertices[it.v[i].index].normal * mtInvT;
                v.color = vertices[it.v[i].index].color;
                v.u = it.v[i].u;
                v.v = it.v[i].v;
                vIndex++;
            }
            renderTriangle.textureId = it.textureId;
            renderTriangles.push_back(renderTriangle);
        }
    }

}