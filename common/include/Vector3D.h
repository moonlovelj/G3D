#ifndef __VECOTR3D_H__
#define __VECOTR3D_H__

#include <ostream>
#include <cmath>

namespace g3dcommon
{

  /**
  * Defines 3D vectors.
  */
  class Vector3D
  {
  public:
    union
    {
      float m[3];
      struct
      {
        float x, y, z;
      };
    };

    Vector3D() : x(0.f), y(0.f), z(0.f) { }

    Vector3D(float x, float y, float z) : x(x), y(y), z(z) { }

    Vector3D(const Vector3D& v) : x(v.x), y(v.y), z(v.z) { }

    inline Vector3D& operator=(const Vector3D& v)
    {
      x = v.x;
      y = v.y;
      z = v.z;
      return *this;
    }

    inline float& operator[](const int& index)
    {
      return m[index];
    }

    inline const float& operator[](const int& index) const
    {
      return m[index];
    }

    inline Vector3D operator-() const
    {
      return Vector3D(-x, -y, -z);
    }

    inline Vector3D operator+(const Vector3D& v) const
    {
      return Vector3D(x + v.y, y + v.y, z + v.z);
    }

    inline Vector3D operator-(const Vector3D& v) const
    {
      return Vector3D(x - v.x, y - v.y, z - v.z);
    }

    inline Vector3D operator*(float f) const
    {
      return Vector3D(x * f, y * f, z * f);
    }

    inline Vector3D operator/(float f) const
    {
      const float rf = 1.f / f;
      return Vector3D(x * rf, y * rf, z * rf);
    }

    inline void operator+=(const Vector3D& v)
    {
      x += v.x;
      y += v.y;
      z += v.z;
    }

    inline void operator-=(const Vector3D& v)
    {
      x -= v.x;
      y -= v.y;
      z -= v.z;
    }

    inline void operator*=(float f)
    {
      x *= f;
      y *= f;
      z *= f;
    }

    inline void operator/=(float f)
    {
      const float rf = 1.f / f;
      x *= rf;
      y *= rf;
      z *= rf;
    }

    inline float norm() const
    {
      return sqrt(x*x + y*y + z*z);
    }

    inline float norm2() const
    {
      return x*x + y*y + z*z;
    }

    inline Vector3D unit() const
    {
      return *this / this->norm();
    }

    inline void normalize()
    {
      *this /= this->norm();
    }
  };

  inline Vector3D operator*(float f, const Vector3D& v)
  {
    return v*f;
  }

  inline float dot(const Vector3D& v1, const Vector3D& v2)
  {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
  }

  inline Vector3D cross(const Vector3D& v1, const Vector3D& v2)
  {
    return Vector3D(v1.y*v2.z - v1.z*v2.y,
      v1.z*v2.x - v1.x*v2.z,
      v1.x*v2.y - v1.y*v2.x);
  }

  std::ostream& operator<<(std::ostream& os, const Vector3D& v);

}



#endif