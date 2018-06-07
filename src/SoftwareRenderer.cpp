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

  static const Color red(1.f, 0.f, 0.f, 1.f);

  SoftwareRenderer::SoftwareRenderer() :
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
    camera = new Camera(Vector3D(2, 0, -4), Vector3D(0, 0, 0), Vector3D(0, 1, 0), 90, 1, 1000, targetWidth, targetHeight);
    if (scene)
    {
      scene->SetCamera(camera);
    }
  }

  void SoftwareRenderer::SetRenderTarget(unsigned char* target, size_t width, size_t height)
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
    if (NULL != scene)
    {
      scene->Render(this);
    }
    if (camera)
    {
      //camera->MoveForward(0.01f);
    }
  }

  void SoftwareRenderer::Rasterize2DPoint(float x, float y, const Color& color)
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

  void SoftwareRenderer::Rasterize2DLine(float x0, float y0, float x1, float y1, const Color& color)
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

  void SoftwareRenderer::RasterizeTriangle(float x0, float y0, float x1, float y1, float x2, float y2, const Color& color)
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

        bool b1 = (sx - x0)*dY0 - (sy - y0)*dX0 <= FLT_EPSILON;
        bool b2 = (sx - x1)*dY1 - (sy - y1)*dX1 <= FLT_EPSILON;
        bool b3 = (sx - x2)*dY2 - (sy - y2)*dX2 <= FLT_EPSILON;
        if (b1 == b2 && b2 == b3)
        {
          Rasterize2DPoint(static_cast<float>(sx), static_cast<float>(sy), color);
        }
      }
    }
  }

  void SoftwareRenderer::RasterizeTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
  {
    float x0 = v0.newPosition.x, y0 = v0.newPosition.y;
    float x1 = v1.newPosition.x, y1 = v1.newPosition.y;
    float x2 = v2.newPosition.x, y2 = v2.newPosition.y;

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

        float f0 = (sx - x0)*dY0 - (sy - y0)*dX0;
        float f1 = (sx - x1)*dY1 - (sy - y1)*dX1;
        float f2 = (sx - x2)*dY2 - (sy - y2)*dX2;

        bool b0 = f0 <= FLT_EPSILON;
        bool b1 = f1 <= FLT_EPSILON;
        bool b2 = f2 <= FLT_EPSILON;
        if (b0 == b1 && b1 == b2)
        {
          // p = u0*a + u2*b + u2*c;
          float u0 = sqrt(dX1*dX1 + dY1*dY1)*f1;
          float u1 = sqrt(dX2*dX2 + dY2*dY2)*f2;
          float u2 = sqrt(dX0*dX0 + dY0*dY0)*f0;
          float u = 1.f / (u0 + u1 + u2);
          u0 *= u; u1 *= u; u2 *= u;
          Color c = v0.newColor*u0 + v1.newColor * u1 + v2.newColor * u2;
          if (v0.textureIndex > -1)
          {
            Texture* pTexture = TextureManager::GetInstance().GetTexture(v0.textureIndex);
            float tu = v0.u*u0 + v1.u * u1 + v2.u * u2;
            float tv = v0.v*u0 + v1.v * u1 + v2.v * u2;
            c *= (sampler2d->SampleNearest(*pTexture, tu, tv));
          }
          Rasterize2DPoint(static_cast<float>(sx), static_cast<float>(sy), c);
        }
      }
    }
  }

  void SoftwareRenderer::DrawPrimitive(const std::vector<Vertex>& vertices, const std::vector<size_t>& indexs, size_t primitiveNum, EPrimitiveType primitiveType)
  {
    switch (primitiveType)
    {
    case g3dcommon::EPoint:
      break;
    case g3dcommon::ELine:
      break;
    case g3dcommon::ETriangle:
      for (size_t i = 0; i < primitiveNum; i++)
      {
        auto vertex0 = vertices[indexs[i * 3 + 0]];
        auto vertex1 = vertices[indexs[i * 3 + 1]];
        auto vertex2 = vertices[indexs[i * 3 + 2]];

        // back face culling.
        Vector3D n = Cross(vertex1.newPosition - vertex0.newPosition, vertex2.newPosition - vertex0.newPosition);
        if (camera->CullFace(n, vertex0.newPosition))
        {
          continue;
        }

        // Shade triangle, including lighting calculations.
        ShadeTriangle(vertex0, vertex1, vertex2);

        // Perform coordinate transformation.
        vertex0.newPosition = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(vertex0.newPosition)));
        vertex1.newPosition = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(vertex1.newPosition)));
        vertex2.newPosition = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(vertex2.newPosition)));
        //RasterizeTriangle(vertex0.transformedPosition.x, vertex0.transformedPosition.y, vertex1.transformedPosition.x, vertex1.transformedPosition.y, vertex2.transformedPosition.x, vertex2.transformedPosition.y, c);

        switch (shadeModel)
        {
        case g3dcommon::EConstantShade:
        case g3dcommon::EFlatShade:
          RasterizeTriangle(vertex0.newPosition.x, vertex0.newPosition.y, vertex1.newPosition.x, vertex1.newPosition.y, vertex2.newPosition.x, vertex2.newPosition.y, vertex0.newColor);
          break;
        case g3dcommon::EGouraudShade:
          RasterizeTriangle(vertex0, vertex1, vertex2);
          break;
        default:
          break;
        }

      }
      break;
    default:
      break;
    }
  }

  void SoftwareRenderer::KeyboardEvent(int key, int event, unsigned char mods)
  {
    switch (key)
    {
    case G3D_KEY_w:
      if (nullptr != camera)
      {
        Vector3D v = camera->Dir();
        camera->Move(v*0.1f);
      }
      break;
    case G3D_KEY_s:
      if (nullptr != camera)
      {
        Vector3D v = camera->Dir();
        camera->Move(-v*0.1f);
      }
      break;
    case G3D_KEY_a:
      if (nullptr != camera)
      {
        Vector3D v = camera->Right();
        camera->Move(-v*0.1f);
      }
      break;
    case G3D_KEY_d:
      if (nullptr != camera)
      {
        Vector3D v = camera->Right();
        camera->Move(v*0.1f);
      }
      break;
    default:
      break;
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
        camera->RotateBy(dx*0.01f, dy*0.01f);
      }
    }
    mouseX = x;
    mouseY = y;
  }

  void SoftwareRenderer::ShadeTriangle(Vertex& vertex0, Vertex& vertex1, Vertex& vertex2)
  {
    switch (shadeModel)
    {
    case g3dcommon::EConstantShade:
    {
                                    vertex0.newColor = vertex0.color;
    }
      break;
    case g3dcommon::EFlatShade:
    {
                                Vector3D faceNormal = Cross(vertex1.newPosition - vertex0.newPosition, vertex2.newPosition - vertex0.newPosition);
                                faceNormal.Normalize();
                                vertex0.newColor = { 0.f, 0.f, 0.f, 1.f };
                                const auto& lights = scene->GetSceneLights();
                                for (auto light : lights)
                                {
                                  vertex0.newColor += (vertex0.color * light->SampleL(vertex0.newPosition, faceNormal));
                                }
    }
      break;
    case g3dcommon::EGouraudShade:
    {
                                   vertex0.newColor = { 0.f, 0.f, 0.f, 1.f };
                                   vertex1.newColor = { 0.f, 0.f, 0.f, 1.f };
                                   vertex2.newColor = { 0.f, 0.f, 0.f, 1.f };
                                   const auto& lights = scene->GetSceneLights();
                                   for (auto light : lights)
                                   {
                                     vertex0.newColor += (vertex0.color * light->SampleL(vertex0.newPosition, vertex0.normal));
                                     vertex1.newColor += (vertex1.color * light->SampleL(vertex1.newPosition, vertex1.normal));
                                     vertex2.newColor += (vertex2.color * light->SampleL(vertex2.newPosition, vertex2.normal));
                                   }
    }

      break;
    default:
      break;
    }
  }

}