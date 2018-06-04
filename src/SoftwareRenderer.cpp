#include "SoftwareRenderer.h"
#include "Scene.h"
#include "Common.h"
#include <iostream>



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

  void SoftwareRenderer::Rasterize2DPoint(float x, float y, Color color)
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

  void SoftwareRenderer::Rasterize2DLine(float x0, float y0, float x1, float y1, Color color)
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

  void SoftwareRenderer::RasterizeTriangle(float x0, float y0, float x1, float y1, float x2, float y2, Color color)
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

        Vector3D v0 = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(vertex0.transformedPosition)));
        Vector3D v1 = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(vertex1.transformedPosition)));
        Vector3D v2 = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(vertex2.transformedPosition)));
        RasterizeTriangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, red);
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