#include "SoftwareRenderer.h"
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
    if (nullptr != renderTarget)
    {
      for (int i = 0; i < 4 * targetWidth * targetHeight; i += 4)
      {
        renderTarget[i] = 255;
        renderTarget[i + 1] = 255;
        renderTarget[i + 2] = 0;
        renderTarget[i + 3] = 255;
      }
    }
  }


}