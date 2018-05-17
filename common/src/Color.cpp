#include "Color.h"

namespace g3dcommon
{
  void Color::Clamp()
  {
    r = std::min(std::max(r, 0.f), 1.f);
    g = std::min(std::max(g, 0.f), 1.f);
    b = std::min(std::max(b, 0.f), 1.f);
    a = std::min(std::max(a, 0.f), 1.f);
  }

  unsigned int Color::toUint32() const
  {
    unsigned int color = 0;
    unsigned char R = std::min(std::max(static_cast<int>(r * 255), 0), 255);
    unsigned char G = std::min(std::max(static_cast<int>(g * 255), 0), 255);
    unsigned char B = std::min(std::max(static_cast<int>(b * 255), 0), 255);
    unsigned char A = std::min(std::max(static_cast<int>(a * 255), 0), 255);
    color = color | (A << 24) | (B << 16) | (G << 8) | R;
    return color;
  }

  std::ostream& operator<<(std::ostream& os, const Color& c)
  {
    os << "(r=" << c.r;
    os << " g=" << c.g;
    os << " b=" << c.b;
    os << " a=" << c.a;
    os << ")";
    return os;
  }
}