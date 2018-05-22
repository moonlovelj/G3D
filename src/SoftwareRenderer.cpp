#include "SoftwareRenderer.h"
#include "Scene.h"
#include <iostream>


namespace g3dcommon
{

  SoftwareRenderer::SoftwareRenderer() :
    renderTarget(nullptr),
    scene(nullptr)
  {

  }

  SoftwareRenderer::~SoftwareRenderer()
  {

  }

  void SoftwareRenderer::Init()
  {

  }

  void SoftwareRenderer::SetRenderTarget(unsigned char* target, size_t width, size_t height)
  {
    renderTarget = target;
    targetWidth = width;
    targetHeight = height;
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
  }

  void SoftwareRenderer::Rasterize2DPoint(float x, float y, Color color)
  {
    int sx = static_cast<int>(floor(x));
    int sy = static_cast<int>(floor(y));
    if (sx < 0 || sx >= static_cast<int>(targetWidth) || sy < 0 || sy >= static_cast<int>(targetHeight))
    {
      return;
    }
    int index = sy * targetWidth + sx;
    renderTarget[index * 4] = color.R();
    renderTarget[index + 1] = color.G();
    renderTarget[index + 2] = color.B();
    renderTarget[index + 3] = color.A();
  }

  void SoftwareRenderer::Rasterize2DLine(float x0, float y0, float x1, float y1, Color color)
  {

  }


}