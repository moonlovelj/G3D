#ifndef __VECOTR4D_H__
#define __VECOTR4D_H__

#include "Vector3D.h"

#include <ostream>
#include <cmath>

namespace g3dcommon
{

  /**
  * Defines 4D vectors.
  */
  class Vector4D
  {
  public:
    union
    {
      float m[4];
      struct
      {
        float x, y, z, w;
      };
    };

    Vector4D() : x(0.f), y(0.f), z(0.f), w(0.f) { }

    Vector4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

    Vector4D(const Vector4D& v) : x(v.x), y(v.y), z(v.z), w(v.w) { }

    inline float& operator[](const int& index)
    {
      return m[index];
    }

    inline const float& operator[](const int& index) const
    {
      return m[index];
    }

    inline Vector4D operator-() const
    {
      return Vector4D(-x, -y, -z, -w);
    }

    inline Vector4D operator+(const Vector4D& v) const
    {
      return Vector4D(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    inline Vector4D operator-(const Vector4D& v) const
    {
      return Vector4D(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    inline Vector4D operator*(float f) const
    {
      return Vector4D(x * f, y * f, z * f, w * f);
    }

    inline Vector4D operator/(float f) const
    {
      const float rf = 1.f / f;
      return Vector4D(x * rf, y * rf, z * rf, w * rf);
    }

    inline void operator+=(const Vector4D& v)
    {
      x += v.x;
      y += v.y;
      z += v.z;
      w += v.w;
    }

    inline void operator-=(const Vector4D& v)
    {
      x -= v.x;
      y -= v.y;
      z -= v.z;
      w -= v.w;
    }

    inline void operator*=(float f)
    {
      x *= f;
      y *= f;
      z *= f;
      w *= f;
    }

    inline void operator/=(float f)
    {
      const float rf = 1.f / f;
      x *= rf;
      y *= rf;
      z *= rf;
      w *= rf;
    }

    inline float Norm() const
    {
      return sqrt(x*x + y*y + z*z + w*w);
    }

    inline float Norm2() const
    {
      return x*x + y*y + z*z + w*w;
    }

    inline Vector4D Unit() const
    {
      return *this / this->Norm();
    }

    inline void Normalize()
    {
      *this /= this->Norm();
    }

    /**
    * Converts this vector to a 3D vector ignoring the w component.
    */
    Vector3D To3D() const;

    /**
    * Converts this vector to a 3D vector by dividing x, y, and z by w.
    */
    Vector3D ProjectTo3D() const;
  };

  inline Vector4D operator*(float f, const Vector4D& v)
  {
    return v*f;
  }

  inline float Dot(const Vector4D& v1, const Vector4D& v2)
  {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
  }

  std::ostream& operator<<(std::ostream& os, const Vector4D& v);

}



#endif