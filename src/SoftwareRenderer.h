#ifndef __SOFTWARERENDERER_H__
#define __SOFTWARERENDERER_H__

#include "Renderer.h"
#include "Color.h"
#include "Camera.h"

namespace g3dcommon
{
  class Scene;
  class Sampler2D;
  struct Triangle;
  struct Texture;

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
    void Rasterize2DPoint(float x, float y, const Color& color);
    // Rasterize a line.
    void Rasterize2DLine(float x0, float y0, float x1, float y1, const Color& color);
    // Rasterize a triangle.
    void RasterizeTriangle(float x0, float y0, float x1, float y1, float x2, float y2, const Color& color);
    // Rasterize a triangle based on vertex interpolation.
    void RasterizeTriangle(const RenderVertex& v0, const RenderVertex& v1, const RenderVertex& v2, const int textureId);
    // Rendering a triangle mesh with vertex list.
    void RenderTriangleMesh(std::vector<RenderVertex>& vertices, size_t vertexCount, std::list<RenderTriangle>& triangles, size_t triangleCount);
    
    // Keyboard and mouse event callback.
    void KeyboardEvent(int key, int event, unsigned char mods);
    void MouseEvent(int key, int event, unsigned char mods);
    void CursorEvent(float x, float y);

    // Shade triangle.
    void ShadeTriangle(RenderVertex& vertex0, RenderVertex& vertex1, RenderVertex& vertex2);

  private:

    // Render target memory location.
    unsigned char* renderTarget;
    // Render target width and height.
    size_t targetWidth, targetHeight;
    // The scene to be rendered.
    Scene* scene;
    // The camera to be used.
    Camera* camera;
    // 2D sampler.
    Sampler2D* sampler2d;

    // Is or not mouse button down.
    bool bMouseButtonDown;
    // Mouse cursor position.
    float mouseX, mouseY;
  };
}



#endif