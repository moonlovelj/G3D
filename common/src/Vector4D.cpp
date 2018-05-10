#include "Vector4D.h"

namespace g3dcommon
{
  Vector3D Vector4D::To3D() const
  {
    return Vector3D(x, y, z);
  }

  Vector3D Vector4D::ProjectTo3D() const
  {
    float invW = 1.f / w;
    return Vector3D(x * invW, y * invW, z * invW);
  }
  std::ostream& operator<<(std::ostream& os, const Vector4D& v)
  {
    os << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
    return os;
  }
}