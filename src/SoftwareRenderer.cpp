#include "SoftwareRenderer.h"
#include "Scene.h"
#include "Common.h"
#include "Texture.h"
#include "TextureManager.h"
#include <iostream>
#include "Light.h"
#include <SDL.h>


namespace g3dcommon
{
    static int gMipLevel = 0;

    SoftwareRenderer::SoftwareRenderer()
        :
        renderTarget(nullptr),
        scene(nullptr),
        camera(nullptr),
        mouseX(0.f),
        mouseY(0.f),
        bMouseButtonDown(false)
    {
        shadeModel = EGouraudShade;
        sampler2d = new Sampler2D();
    }

    SoftwareRenderer::~SoftwareRenderer()
    {

    }

    void SoftwareRenderer::Init()
    {
        zBuffer.resize(targetWidth * targetHeight, std::numeric_limits<float>::min());
        camera =
            new Camera(Vector3D(0, 0, -7), Vector3D(0, 0, 0), Vector3D(0, 1, 0), 60, 1, 500, targetWidth, targetHeight);
        if (scene)
        {
            scene->SetCamera(camera);
        }
    }

    void SoftwareRenderer::SetRenderTarget(unsigned char *target, size_t width, size_t height)
    {
        renderTarget = target;
        targetWidth = width;
        targetHeight = height;
        Init();
    }

    void SoftwareRenderer::Resize(size_t w, size_t h)
    {

    }

    void SoftwareRenderer::Render()
    {
        zBuffer.assign(zBuffer.size(), std::numeric_limits<float>::min());
        if (NULL != scene)
        {
            scene->Render(this);
        }
        if (camera)
        {
            //camera->MoveForward(0.01f);
        }
    }

    void SoftwareRenderer::Rasterize2DPoint(float x, float y, const Color &color)
    {
        int sx = static_cast<int>(floor(x + 0.5f));
        int sy = static_cast<int>(floor(targetHeight - 1 - y + 0.5f));
        if (sx < 0 || sx >= static_cast<int>(targetWidth) || sy < 0 || sy >= static_cast<int>(targetHeight))
        {
            return;
        }
        int index = sy * targetWidth + sx;
        renderTarget[index * 4] = color.R();
        renderTarget[index * 4 + 1] = color.G();
        renderTarget[index * 4 + 2] = color.B();
        renderTarget[index * 4 + 3] = color.A();
    }

    void SoftwareRenderer::Rasterize2DPointWithZ(float x, float y, float z, const Color &color)
    {
        int sx = static_cast<int>(floor(x + 0.5f));
        int sy = static_cast<int>(floor(targetHeight - 1 - y + 0.5f));
        if (sx < 0 || sx >= static_cast<int>(targetWidth) || sy < 0 || sy >= static_cast<int>(targetHeight))
        {
            return;
        }
        int index = sy * targetWidth + sx;
        if (z < zBuffer[index])
        {
            return;
        }
        zBuffer[index] = z;
        renderTarget[index * 4] = color.R();
        renderTarget[index * 4 + 1] = color.G();
        renderTarget[index * 4 + 2] = color.B();
        renderTarget[index * 4 + 3] = color.A();
    }

    void SoftwareRenderer::Rasterize2DLine(float x0, float y0, float x1, float y1, const Color &color)
    {
        int xStart = static_cast<int>(x0 + 0.5f);
        int yStart = static_cast<int>(y0 + 0.5f);
        int xEnd = static_cast<int>(x1 + 0.5f);
        int yEnd = static_cast<int>(y1 + 0.5f);

        bool steep = abs(yEnd - yStart) > abs(xEnd - xStart);
        if (steep)
        {
            std::swap(xStart, yStart);
            std::swap(xEnd, yEnd);
        }
        if (xStart > xEnd)
        {
            std::swap(xStart, xEnd);
            std::swap(yStart, yEnd);
        }
        int x = xStart;
        int y = yStart;
        if (xStart == xEnd)
        {
            while (y <= yEnd)
            {
                Rasterize2DPoint(static_cast<float>(x), static_cast<float>(y), color);
                y++;
            }
        }
        else
        {
            int ystep = yStart < yEnd ? 1 : -1;
            int dx = xEnd - xStart;
            int dy = yEnd - yStart;
            int twoDy = 2 * dy;
            int twoDyMinusDx = 2 * (dy - dx);
            int twoDyPlusDx = 2 * (dy + dx);
            int p = twoDy - ystep * dx;
            while (x <= xEnd)
            {
                if (steep)
                {
                    Rasterize2DPoint(static_cast<float>(y), static_cast<float>(x), color);
                }
                else
                {
                    Rasterize2DPoint(static_cast<float>(x), static_cast<float>(y), color);
                }

                x++;

                if (p < 0)
                {
                    if (yStart < yEnd)
                    {
                        p += twoDy;
                    }
                    else
                    {
                        p += twoDyPlusDx;
                        y--;
                    }
                }
                else
                {
                    if (yStart < yEnd)
                    {
                        p += twoDyMinusDx;
                        y++;
                    }
                    else
                    {
                        p += twoDy;
                    }
                }
            }
        }
    }

    void
    SoftwareRenderer::RasterizeTriangle(float x0, float y0, float x1, float y1, float x2, float y2, const Color &color)
    {
        if ((x0 < 0 && x1 < 0 && x2 < 0) ||
            (y0 < 0 && y1 < 0 && y2 < 0) ||
            (x0 >= targetWidth && x1 >= targetWidth && x2 >= targetWidth) ||
            (y0 >= targetHeight && y1 >= targetHeight && y2 >= targetHeight))
            return;

        float dY0 = y1 - y0, dY1 = y2 - y1, dY2 = y0 - y2;
        float dX0 = x1 - x0, dX1 = x2 - x1, dX2 = x0 - x2;

        int minX = static_cast<int>(std::min(std::min(x0, x1), x2) + 0.5f);
        int minY = static_cast<int>(std::min(std::min(y0, y1), y2) + 0.5f);
        int maxX = static_cast<int>(std::max(std::max(x0, x1), x2) + 0.5f);
        int maxY = static_cast<int>(std::max(std::max(y0, y1), y2) + 0.5f);

        for (int sy = minY; sy <= maxY; sy++)
        {
            for (int sx = minX; sx <= maxX; sx++)
            {
                if (sx < 0 || sx >= static_cast<int>(targetWidth)) continue;
                if (sy < 0 || sy >= static_cast<int>(targetHeight)) continue;

                bool b1 = (sx - x0) * dY0 - (sy - y0) * dX0 <= EPS_F;
                bool b2 = (sx - x1) * dY1 - (sy - y1) * dX1 <= EPS_F;
                bool b3 = (sx - x2) * dY2 - (sy - y2) * dX2 <= EPS_F;
                if (b1 == b2 && b2 == b3)
                {
                    Rasterize2DPoint(static_cast<float>(sx), static_cast<float>(sy), color);
                }
            }
        }
    }

    void SoftwareRenderer::RasterizeTriangle(const RenderVertex &v0,
                                             const RenderVertex &v1,
                                             const RenderVertex &v2,
                                             const Color &color)
    {
        float x0 = v0.position.x, y0 = v0.position.y, z0 = v0.position.z;
        float x1 = v1.position.x, y1 = v1.position.y, z1 = v1.position.z;
        float x2 = v2.position.x, y2 = v2.position.y, z2 = v2.position.z;

        if ((x0 < 0 && x1 < 0 && x2 < 0) ||
            (y0 < 0 && y1 < 0 && y2 < 0) ||
            (x0 >= targetWidth && x1 >= targetWidth && x2 >= targetWidth) ||
            (y0 >= targetHeight && y1 >= targetHeight && y2 >= targetHeight))
            return;

        float dY0 = y1 - y0, dY1 = y2 - y1, dY2 = y0 - y2;
        float dX0 = x1 - x0, dX1 = x2 - x1, dX2 = x0 - x2;

        int minX = static_cast<int>(std::min(std::min(x0, x1), x2) + 0.5f);
        int minY = static_cast<int>(std::min(std::min(y0, y1), y2) + 0.5f);
        int maxX = static_cast<int>(std::max(std::max(x0, x1), x2) + 0.5f);
        int maxY = static_cast<int>(std::max(std::max(y0, y1), y2) + 0.5f);

        for (int sy = minY; sy <= maxY; sy++)
        {
            for (int sx = minX; sx <= maxX; sx++)
            {
                if (sx < 0 || sx >= static_cast<int>(targetWidth)) continue;
                if (sy < 0 || sy >= static_cast<int>(targetHeight)) continue;

                float f0 = (sx - x0) * dY0 - (sy - y0) * dX0;
                float f1 = (sx - x1) * dY1 - (sy - y1) * dX1;
                float f2 = (sx - x2) * dY2 - (sy - y2) * dX2;

                bool b0 = f0 <= EPS_F;
                bool b1 = f1 <= EPS_F;
                bool b2 = f2 <= EPS_F;
                if (b0 == b1 && b1 == b2)
                {
                    float u0 = f1;
                    float u1 = f2;
                    float u2 = f0;
                    float u = 1.f / (u0 + u1 + u2);
                    u0 *= u;
                    u1 *= u;
                    u2 *= u;
                    float z = z0 * u0 + z1 * u1 + z2 * u2;
                    Rasterize2DPointWithZ(static_cast<float>(sx), static_cast<float>(sy), z, color);
                }
            }
        }
    }

    void SoftwareRenderer::RasterizeTriangle(const RenderVertex &v0,
                                             const RenderVertex &v1,
                                             const RenderVertex &v2,
                                             const int textureId)
    {
        float x0 = v0.position.x, y0 = v0.position.y, z0 = v0.position.z;
        float x1 = v1.position.x, y1 = v1.position.y, z1 = v1.position.z;
        float x2 = v2.position.x, y2 = v2.position.y, z2 = v2.position.z;

        if ((x0 < 0 && x1 < 0 && x2 < 0) ||
            (y0 < 0 && y1 < 0 && y2 < 0) ||
            (x0 >= targetWidth && x1 >= targetWidth && x2 >= targetWidth) ||
            (y0 >= targetHeight && y1 >= targetHeight && y2 >= targetHeight))
            return;

        float dY0 = y1 - y0, dY1 = y2 - y1, dY2 = y0 - y2;
        float dX0 = x1 - x0, dX1 = x2 - x1, dX2 = x0 - x2;

        int minX = static_cast<int>(std::min(std::min(x0, x1), x2) + 0.5f);
        int minY = static_cast<int>(std::min(std::min(y0, y1), y2) + 0.5f);
        int maxX = static_cast<int>(std::max(std::max(x0, x1), x2) + 0.5f);
        int maxY = static_cast<int>(std::max(std::max(y0, y1), y2) + 0.5f);

        for (int sy = minY; sy <= maxY; sy++)
        {
            for (int sx = minX; sx <= maxX; sx++)
            {
                if (sx < 0 || sx >= static_cast<int>(targetWidth)) continue;
                if (sy < 0 || sy >= static_cast<int>(targetHeight)) continue;

                float f0 = (sx - x0) * dY0 - (sy - y0) * dX0;
                float f1 = (sx - x1) * dY1 - (sy - y1) * dX1;
                float f2 = (sx - x2) * dY2 - (sy - y2) * dX2;

                bool b0 = f0 <= EPS_F;
                bool b1 = f1 <= EPS_F;
                bool b2 = f2 <= EPS_F;
                if (b0 == b1 && b1 == b2)
                {
                    // p = u0*a + u1*b + u2*c;
                    float u0 = f1;
                    float u1 = f2;
                    float u2 = f0;

                    float u = 1.f / (u0 + u1 + u2);
                    u0 *= u;
                    u1 *= u;
                    u2 *= u;
                    Color c = v0.color * u0 + v1.color * u1 + v2.color * u2;
                    float z = z0 * u0 + z1 * u1 + z2 * u2;
                    if (textureId > -1)
                    {
                        Texture *pTexture = TextureManager::GetInstance().GetTexture(textureId);
                        // Perfect perspective texture correction.
                        float tu = (v0.u * u0 * z0 + v1.u * u1 * z1 + v2.u * u2 * z2) / z;
                        float tv = (v0.v * u0 * z0 + v1.v * u1 * z1 + v2.v * u2 * z2) / z;
                        c *= (sampler2d->SampleBilinear(*pTexture, tu, tv, gMipLevel));
                    }
                    Rasterize2DPointWithZ(static_cast<float>(sx), static_cast<float>(sy), z, c);
                }
            }
        }
    }

    void SoftwareRenderer::RenderTriangleMesh(std::vector<RenderVertex> &vertices,
                                              size_t vertexCount,
                                              std::list<RenderTriangle> &triangles,
                                              size_t triangleCount)
    {
        for (auto &triangle : triangles)
        {
            auto vertex0 = vertices[triangle.indexs[0]];
            auto vertex1 = vertices[triangle.indexs[1]];
            auto vertex2 = vertices[triangle.indexs[2]];
            // back face culling.
            Vector3D n = Cross(vertex1.position - vertex0.position, vertex2.position - vertex0.position);
            if (camera->CullFace(n, vertex0.position))
            {
                continue;
            }

            // Shade triangle, including lighting calculations.
            ShadeTriangle(vertex0, vertex1, vertex2);
            // Perform coordinate transformation.
            vertex0.position =
                camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(vertex0.position)));
            vertex1.position =
                camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(vertex1.position)));
            vertex2.position =
                camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(vertex2.position)));
            //Rasterize2DLine(vertex0.position.x, vertex0.position.y, vertex1.position.x, vertex1.position.y, vertex0.color);
            //Rasterize2DLine(vertex1.position.x, vertex1.position.y, vertex2.position.x, vertex2.position.y, vertex0.color);
            //Rasterize2DLine(vertex2.position.x, vertex2.position.y, vertex0.position.x, vertex0.position.y, vertex0.color);

            switch (shadeModel)
            {
                case g3dcommon::EConstantShade:
                case g3dcommon::EFlatShade:
                    //RasterizeTriangle(vertex0, vertex1, vertex2, vertex0.color);
                    RasterizeTriangle(vertex0, vertex1, vertex2, triangle.textureId);
                    break;
                case g3dcommon::EGouraudShade:RasterizeTriangle(vertex0, vertex1, vertex2, triangle.textureId);
                    break;
                default:break;
            }
        }
    }

    void SoftwareRenderer::KeyboardEvent(int key, int event, unsigned char mods)
    {
        switch (key)
        {
            case G3D_KEY_w:
                if (nullptr != camera)
                {
                    camera->MoveForward(0.5f);
                }
                break;
            case G3D_KEY_s:
                if (nullptr != camera)
                {
                    camera->MoveForward(-0.5f);
                }
                break;
            case G3D_KEY_a:
                if (nullptr != camera)
                {
                    Vector3D v = camera->Right();
                    camera->Move(-v * 0.5f);
                }
                break;
            case G3D_KEY_d:
                if (nullptr != camera)
                {
                    Vector3D v = camera->Right();
                    camera->Move(v * 0.5f);
                }
                break;
            case G3D_KEY_m:
            {
                gMipLevel++;
            }
                break;
            case G3D_KEY_n:
            {
                gMipLevel--;
            }
                break;
            default:break;
        }
    }

    void SoftwareRenderer::MouseEvent(int key, int event, unsigned char mods)
    {
        if (event == SDL_MOUSEBUTTONDOWN)
        {
            bMouseButtonDown = true;
        }
        else
        {
            bMouseButtonDown = false;
        }
    }

    void SoftwareRenderer::CursorEvent(float x, float y)
    {
        if (bMouseButtonDown)
        {
            float dx = x - mouseX;
            float dy = y - mouseY;
            if (nullptr != camera)
            {
                camera->RotateBy(dx * 0.01f, dy * 0.01f);
            }
        }
        mouseX = x;
        mouseY = y;
    }

    void SoftwareRenderer::ShadeTriangle(RenderVertex &vertex0, RenderVertex &vertex1, RenderVertex &vertex2)
    {
        switch (shadeModel)
        {
            case g3dcommon::EConstantShade:
            {

            }
                break;
            case g3dcommon::EFlatShade:
            {
                Vector3D faceNormal = Cross(vertex1.position - vertex0.position, vertex2.position - vertex0.position);
                faceNormal.Normalize();
                Color c = {0.f, 0.f, 0.f, 1.f};
                const auto &lights = scene->GetSceneLights();
                for (auto light : lights)
                {
                    c += (vertex0.color * light->SampleL(vertex0.position, faceNormal));
                }
                vertex0.color = c;
            }
                break;
            case g3dcommon::EGouraudShade:
            {
                Color c0 = {0.f, 0.f, 0.f, 1.f};
                Color c1 = {0.f, 0.f, 0.f, 1.f};
                Color c2 = {0.f, 0.f, 0.f, 1.f};
                const auto &lights = scene->GetSceneLights();
                for (auto light : lights)
                {
                    c0 += (vertex0.color * light->SampleL(vertex0.position, vertex0.normal));
                    c1 += (vertex1.color * light->SampleL(vertex1.position, vertex1.normal));
                    c2 += (vertex2.color * light->SampleL(vertex2.position, vertex2.normal));
                }
                vertex0.color = c0;
                vertex1.color = c1;
                vertex2.color = c2;
            }

                break;
            default:break;
        }
    }

}
