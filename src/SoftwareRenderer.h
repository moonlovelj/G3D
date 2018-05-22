#ifndef __SOFTWARERENDERER_H__
#define __SOFTWARERENDERER_H__

#include "Renderer.h"
#include "Color.h"

namespace g3dcommon
{
  class Scene;

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
    // Set the scene to be rendered.
    void SetRenderScene(Scene* s) { scene = s; }

    // Rasterize a point.
    void Rasterize2DPoint(float x, float y, Color color);
    // Rasterize a line.
    void Rasterize2DLine(float x0, float y0, float x1, float y1, Color color);

  private:

    // Render target memory location.
    unsigned char* renderTarget;
    // Render target width and height.
    size_t targetWidth, targetHeight;
    // The scene to be rendered.
    Scene* scene;
  };
}



#endif