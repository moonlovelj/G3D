#ifndef __SOFTWARERENDERER_H__
#define __SOFTWARERENDERER_H__

#include "Renderer.h"
#include "Color.h"

namespace g3dcommon
{
  class SoftwareRenderer : public Renderer
  {
  public:
    SoftwareRenderer();
    ~SoftwareRenderer();

    // Initialize the renderer.
    void Init();
    // Set render target.
    void SetRenderTarget(unsigned char* target, size_t width, size_t height);
    // Resize buffer.
    void Resize(size_t w, size_t h);
    // Draw content.
    void Render();

    // Rasterize a point.
    void RasterizePoint(float x, float y, Color color);

  private:

    // Render target memory location.
    unsigned char* renderTarget;
    // Render target width and height.
    size_t targetWidth, targetHeight;
  };
}



#endif