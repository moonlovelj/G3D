#include "Vector2D.h"

namespace g3dcommon
{
  std::ostream& operator<<(std::ostream& os, const Vector2D& v)
  {
    os << "(" << v.x << "," << v.y << ")";
    return os;
  }
}