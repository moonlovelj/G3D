#include "SoftwareRenderer.h"
#include "Scene.h"
#include "Common.h"
#include <iostream>
#include "Light.h"



namespace g3dcommon
{

  static const Color red(1.f,0.f,0.f,1.f);

  SoftwareRenderer::SoftwareRenderer() :
    renderTarget(nullptr),
    scene(nullptr),
    camera(nullptr)
  {

  }

  SoftwareRenderer::~SoftwareRenderer()
  {

  }

  void SoftwareRenderer::Init()
  {
    camera = new Camera(Vector3D(2,0,-4), Vector3D(0, 0, 0), Vector3D(0, 1, 0), 90, 1, 1000, targetWidth, targetHeight);
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
    renderTarget[index * 4  + 1] = color.G();
    renderTarget[index * 4  + 2] = color.B();
    renderTarget[index * 4  + 3] = color.A();
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
    float x0 = v0.transformedPosition.x, y0 = v0.transformedPosition.y;
    float x1 = v1.transformedPosition.x, y1 = v1.transformedPosition.y;
    float x2 = v2.transformedPosition.x, y2 = v2.transformedPosition.y;

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
          Color c = v0.color*u0 + v1.color * u1 + v2.color * u2;
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
        size_t index0 = indexs[i * 3 + 0];
        size_t index1 = indexs[i * 3 + 1];
        size_t index2 = indexs[i * 3 + 2];
        Vertex vertex0 = vertices[index0];
        Vertex vertex1 = vertices[index1];
        Vertex vertex2 = vertices[index2];

        // back face culling.
        Vector3D n = Cross(vertex1.transformedPosition - vertex0.transformedPosition, vertex2.transformedPosition - vertex0.transformedPosition);
        if (camera->CullFace(n, vertex0.transformedPosition))
        {
          continue;
        }

        /*auto lights = scene->GetSceneLights();
        Color c;
        for (auto light : lights)
        {
        Vector3D wi;
        Color sample = light->SampleL(vertex0.transformedPosition, &wi);
        float cosTheat = Dot(n.Unit(), wi);
        if (cosTheat > 0.f)
        {
        c += (vertex0.color * sample * cosTheat);
        }
        }
        c.a = 1.f;*/
        Color c0, c1, c2;
        const auto& lights = scene->GetSceneLights();
        for (auto light : lights)
        {
          Vector3D wi;
          Color sample = light->SampleL(vertex0.transformedPosition, &wi);
          float cosTheat = Dot(vertex0.normal, wi);
          if (cosTheat > 0.f)
          {
            c0 += (vertex0.color * sample * cosTheat);
          }

          sample = light->SampleL(vertex1.transformedPosition, &wi);
          cosTheat = Dot(vertex1.normal, wi);
          if (cosTheat > 0.f)
          {
            c1 += (vertex1.color * sample * cosTheat);
          }

          sample = light->SampleL(vertex2.transformedPosition, &wi);
          cosTheat = Dot(vertex2.normal, wi);
          if (cosTheat > 0.f)
          {
            c2 += (vertex2.color * sample * cosTheat);
          }
        }
        vertex0.color = c0;
        vertex1.color = c1;
        vertex2.color = c2;

        vertex0.transformedPosition = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(vertex0.transformedPosition)));
        vertex1.transformedPosition = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(vertex1.transformedPosition)));
        vertex2.transformedPosition = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(vertex2.transformedPosition)));
        //RasterizeTriangle(vertex0.transformedPosition.x, vertex0.transformedPosition.y, vertex1.transformedPosition.x, vertex1.transformedPosition.y, vertex2.transformedPosition.x, vertex2.transformedPosition.y, c);

        RasterizeTriangle(vertex0, vertex1, vertex2);
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

}