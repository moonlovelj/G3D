#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "Vector3D.h"

namespace g3dcommon
{
  class Quaternion
  {
  public:
    union
    {
      float m[4];
//      struct
//      {
//        float q0;
//        Vector3D qv;
//      };
      struct
      {
        float w, x, y, z;
      };
    };

    /**
    * Constructor.
    */
    Quaternion() : w(1.f), x(0.f), y(0.f), z(0.f) { }

    Quaternion(const Quaternion& q) : w(q.w), x(q.x), y(q.y), z(q.z) { }

    //Quaternion(float q0, const Vector3D& qv) : q0(q0), qv(qv) { }

    Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) { }

    /**
     * Initializes a quaternion that represents a rotation about the given axis
     * and angle.
     */
    void FromAxisAngle(const Vector3D& axis, float radians)
    {
      radians *= 0.5f;
      const Vector3D& nAxis = axis.Unit();
      float sinTheta = sin(radians);
      w = cos(radians);
      x = sinTheta * nAxis.x;
      y = sinTheta * nAxis.y;
      z = sinTheta * nAxis.z;
      this->Normalize();
    }

    /**
     * Initializes a quaternion that represents a rotation about the given euler.
     */
    void FromEuler(float radiansX, float radiansY, float radiansZ)
    {
      radiansX *= 0.5f; radiansY *= 0.5f; radiansZ *= 0.5f;

      float cosZ2 = cos(radiansZ);
      float cosY2 = cos(radiansY);
      float cosX2 = cos(radiansX);

      float sinZ2 = sin(radiansZ);
      float sinY2 = sin(radiansY);
      float sinX2 = sin(radiansX);

      w = cosZ2*cosY2*cosX2 + sinZ2*sinY2*sinX2;
      x = cosZ2*cosY2*sinX2 - sinZ2*sinY2*cosX2;
      y = cosZ2*sinY2*cosX2 + sinZ2*cosY2*sinX2;
      z = sinZ2*cosY2*cosX2 - cosZ2*sinY2*sinX2;
    }

    inline float q0() const
    {
      return w;
    }

    inline Vector3D qv() const
    {
      return Vector3D(x,y,z);
    }

    inline Quaternion operator-() const
    {
      return Quaternion(-w, -x, -y, -z);
    }

    inline Quaternion operator+(const Quaternion& q) const
    {
      return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
    }

    inline Quaternion operator-(const Quaternion& q) const
    {
      return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
    }

    inline Quaternion operator*(float f) const
    {
      return Quaternion(w * f, x * f, y * f, z * f);
    }

    inline Quaternion operator/(float f) const
    {
      float rf = 1.f / f;
      return Quaternion(w * rf, x * rf, y * rf, z * rf);
    }

    inline void operator+=(const Quaternion& q)
    {
      w += q.w; x += q.x; y += q.y; z += q.z;
    }

    inline void operator-=(const Quaternion& q)
    {
      w -= q.w; x -= q.x; y -= q.y; z -= q.z;
    }

    inline void operator*=(float f)
    {
      w *= f; x *= f; y *= f; z *= f;
    }

    inline void operator/=(float f)
    {
      *this *= (1.f / f);
    }

    /**
     * Quaternion multiplication.
     * (p0, pv) * (q0, qv) = (p0*q0 - dot(pv,qv), p0*qv + q0*pv + cross(pv,qv)).
     */
    Quaternion operator*(const Quaternion& q) const
    {
      Vector3D qVector3 = q0()*q.qv() + q.q0()*qv() + Cross(qv(), q.qv());
      return Quaternion(q0()*q.q0() - Dot(qv(), q.qv()), qVector3.x, qVector3.y, qVector3.z);
    }

    inline float Norm() const
    {
      return sqrt(w*w + x*x + y*y + z*z);
    }

    inline float Norm2() const
    {
      return w*w + x*x + y*y + z*z;
    }

    inline void Normalize()
    {
      *this /= Norm();
    }

    Vector3D Complex() const 
    { 
      return Vector3D(x,y,z);
    }
    void setComplex(const Vector3D& c) 
    { 
      x = c.x; y = c.y; z = c.z; 
    }

    float Real() const 
    { 
      return w; 
    }
    void SetReal(float r) 
    { 
      w = r; 
    }

    /**
     * Returns conjugate quaternion.
     */
    Quaternion Conjugate() const
    {
      return Quaternion(w, -x, -y, -z);
    }

    /**
    * Computes the inverse of this quaternion.
    *
    * This is a general inverse.  If this is
    * a unit quaternion (i.e., norm() == 1),
    * it will be significantly faster to use conjugate() instead.
    *
    * @return The quaternion q such that q * (*this) == (*this) * q
    * == [ 1 0 0 0 ].
    */
    Quaternion Inverse() const
    {
      return Conjugate() / Norm2();
    }

    /**
    * Returns a vector rotated by this quaternion.
    *
    * Functionally equivalent to:  (v * RotationMatrix())
    * or (q * Quaternion(0, v) * q.Inverse()).
    *
    * @warning Conjugate() is used instead of inverse() for better
    * performance, when this quaternion must be normalized.
    */
    Vector3D RotatedVector(const Vector3D& v) const
    {
      return (((*this) * Quaternion(0, v.x, v.y, v.z)) * Conjugate()).Complex();
    }
  };

  inline Quaternion operator*(float f, const Quaternion& q)
  {
    return q * f;
  }

  std::ostream& operator<<(std::ostream& os, const Quaternion& q);
}

#endif