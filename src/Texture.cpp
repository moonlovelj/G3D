#include "Texture.h"

namespace g3dcommon
{


  Sampler2D::Sampler2D(SampleMethod method)
    : method(method)
  {

  }

  Sampler2D::~Sampler2D()
  {

  }

  Color Sampler2D::SampleNearest(const Texture& tex, float u, float v)
  {
    int x = static_cast<int>(floor(tex.width * u));
    int y = static_cast<int>(floor(tex.height * v));
    x = std::min(std::max(x, 0), static_cast<int>(tex.width - 1));
    y = std::min(std::max(y, 0), static_cast<int>(tex.height - 1));
    size_t index = 4 * (y * tex.width + x);
    return Color(tex.texels[index], tex.texels[index + 1], tex.texels[index + 2], tex.texels[index + 3]);
  }

}
