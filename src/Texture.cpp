#include "Texture.h"
#include "Common.h"
#include <cmath>

namespace g3dcommon
{

  inline Color Sip(const Texture& tex, int x, int y)
  {
    x = std::min(std::max(0, x), static_cast<int>(tex.width - 1));
    y = std::min(std::max(0, y), static_cast<int>(tex.height - 1));
    int index = 4 * (y * tex.width + x);
    return Color(tex.texels[index], tex.texels[index + 1], tex.texels[index + 2], tex.texels[index + 3]);
  }

  Sampler2D::Sampler2D(SampleMethod method)
    : method(method)
  {

  }

  Sampler2D::~Sampler2D()
  {

  }

  Color Sampler2D::SampleNearest(const Texture& tex, float u, float v)
  {
    int x = static_cast<int>(std::floor((tex.width - 1) * u + 0.5f));
    int y = static_cast<int>(std::floor((tex.height - 1) * v + 0.5f));
    return Sip(tex, x, y);
  }

  Color Sampler2D::SampleBilinear(const Texture& tex, float u, float v)
  {
    float x = (tex.width - 1) * u + 0.5f;
    float y = (tex.height - 1) * v + 0.5f;
    int sx = std::floor(x), sy = std::floor(y);
    float tx = x - sx, ty = y - sy;
    Color c1 = Interpolate(Sip(tex,sx,sy), Sip(tex,sx+1,sy) ,tx);
    Color c2 = Interpolate(Sip(tex,sx,sy+1), Sip(tex,sx+1,sy+1) ,tx);
    return Interpolate(c1, c2, ty);
  }

}
