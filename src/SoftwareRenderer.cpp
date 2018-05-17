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
    memset(renderTarget, 0x8F, 4 * targetWidth * targetHeight);
    /*if (nullptr != renderTarget)
    {
    for (size_t i = 0; i < 4 * targetWidth * targetHeight; i += 4)
    {
    renderTarget[i] = c.R();
    renderTarget[i + 1] = c.G();
    renderTarget[i + 2] = c.B();
    renderTarget[i + 3] = c.A();
    }
    }*/
  }


}