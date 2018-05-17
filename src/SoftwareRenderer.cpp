#include "SoftwareRenderer.h"
#include <iostream>

namespace g3dcommon
{

  SoftwareRenderer::SoftwareRenderer() :
    renderTarget(nullptr)
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
    for (int i = 100; i < 400;i++)
    {
      RasterizePoint(i, 100, Color(1.f, 0.f, 0.f, 1.f));
    }
    
  }

  void SoftwareRenderer::RasterizePoint(float x, float y, Color color)
  {
    int sx = static_cast<int>(floor(x));
    int sy = static_cast<int>(floor(y));
    if (sx < 0 || sx >= targetWidth || sy < 0 || sy >= targetHeight)
    {
      return;
    }
    int index = sy * targetWidth + sx;
    renderTarget[index * 4] = color.R();
    renderTarget[index + 1] = color.G();
    renderTarget[index + 2] = color.B();
    renderTarget[index + 3] = color.A();
  }


}