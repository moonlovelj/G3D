#include "Quaternion.h"

namespace g3dcommon
{
  std::ostream& operator<<(std::ostream& os, const Quaternion& q)
  {
    os << "{ " << q.w << ", " << q.x << "i, " << q.y << "j, " << q.z << "k }";
    return os;
  }
}