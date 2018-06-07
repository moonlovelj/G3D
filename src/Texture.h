#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Color.h"
#include <vector>

namespace g3dcommon
{
  enum SampleMethod
  {
    NEAREST,
    BILINEAR,
    TRILINEAR
  };

  struct Texture 
  {
    size_t width;
    size_t height;
    std::string name;
    std::vector<unsigned char> texels;
  };

  class Sampler2D
  {
  public:
    Sampler2D(SampleMethod method = NEAREST);
    ~Sampler2D();

    Color SampleNearest(const Texture& tex, float u, float v);

  private:

    SampleMethod method;
  };

}

#endif