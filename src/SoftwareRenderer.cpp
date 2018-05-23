#include "SoftwareRenderer.h"
#include "Scene.h"
#include <iostream>


namespace g3dcommon
{

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
    camera = new Camera(Vector3D(2, 3, -5), Vector3D(0, 0, 0), Vector3D(0, 1, 0), 90, 1, 1000, targetWidth, targetHeight);
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

    Vector3D v[8];
    v[0] = Vector3D(-1,-1,-1);
    v[1] = Vector3D(-1, 1, -1);
    v[2] = Vector3D(1, 1, -1);
    v[3] = Vector3D(1, -1, -1);
    v[4] = Vector3D(1, -1, 1);
    v[5] = Vector3D(1, 1, 1);
    v[6] = Vector3D(-1, 1, 1);
    v[7] = Vector3D(-1, -1, 1);
    
    Vector3D vp[8];
    vp[0] = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(v[0])));
    vp[1] = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(v[1])));
    vp[2] = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(v[2])));
    vp[3] = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(v[3])));
    vp[4] = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(v[4])));
    vp[5] = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(v[5])));
    vp[6] = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(v[6])));
    vp[7] = camera->ConvertViewToScreen(camera->ProjectToView(camera->ConvertWorldToCamera(v[7])));

    Rasterize2DLine(vp[0].x, vp[0].y, vp[1].x, vp[1].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[1].x, vp[1].y, vp[2].x, vp[2].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[0].x, vp[0].y, vp[2].x, vp[2].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[0].x, vp[0].y, vp[3].x, vp[3].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[2].x, vp[2].y, vp[3].x, vp[3].y, Color(0.f, 1.f, 0.f, 1.f));

    Rasterize2DLine(vp[4].x, vp[4].y, vp[5].x, vp[5].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[4].x, vp[4].y, vp[7].x, vp[7].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[5].x, vp[5].y, vp[6].x, vp[6].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[6].x, vp[6].y, vp[7].x, vp[7].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[5].x, vp[5].y, vp[7].x, vp[7].y, Color(0.f, 1.f, 0.f, 1.f));

    Rasterize2DLine(vp[0].x, vp[0].y, vp[1].x, vp[1].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[0].x, vp[0].y, vp[7].x, vp[7].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[1].x, vp[1].y, vp[6].x, vp[6].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[6].x, vp[6].y, vp[7].x, vp[7].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[0].x, vp[0].y, vp[6].x, vp[6].y, Color(0.f, 1.f, 0.f, 1.f));

    Rasterize2DLine(vp[2].x, vp[2].y, vp[3].x, vp[3].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[2].x, vp[2].y, vp[5].x, vp[5].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[4].x, vp[4].y, vp[3].x, vp[3].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[4].x, vp[4].y, vp[5].x, vp[5].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[3].x, vp[3].y, vp[5].x, vp[5].y, Color(0.f, 1.f, 0.f, 1.f));

    Rasterize2DLine(vp[2].x, vp[2].y, vp[6].x, vp[6].y, Color(0.f, 1.f, 0.f, 1.f));
    Rasterize2DLine(vp[7].x, vp[7].y, vp[3].x, vp[3].y, Color(0.f, 1.f, 0.f, 1.f));
  }

  void SoftwareRenderer::Rasterize2DPoint(float x, float y, Color color)
  {
    int sx = static_cast<int>(floor(x));
    int sy = static_cast<int>(floor(targetHeight - 1 - y));
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
    int xStart = static_cast<int>(x0);
    int yStart = static_cast<int>(y0);
    int xEnd = static_cast<int>(x1);
    int yEnd = static_cast<int>(y1);

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
        Rasterize2DPoint(x, y, color);
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
          Rasterize2DPoint(y, x, color);
        }
        else
        {
          Rasterize2DPoint(x, y, color);
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


}