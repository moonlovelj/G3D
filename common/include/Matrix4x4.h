#ifndef __Matrix4x4_H__
#define __Matrix4x4_H__

#include "Vector3D.h"
#include "Vector4D.h"
#include <ostream>
namespace g3dcommon
{
  class Matrix4x4
  {
  public:
    enum class Axis { X, Y, Z };

    Matrix4x4():m00(0.f), m01(0.f), m02(0.f), m03(0.f),
                m10(0.f), m11(0.f), m12(0.f), m13(0.f),
                m20(0.f), m21(0.f), m22(0.f), m23(0.f),
                m30(0.f), m31(0.f), m32(0.f), m33(0.f) { }
    
    Matrix4x4(float m00, float m01, float m02, float m03,
              float m10, float m11, float m12, float m13,
              float m20, float m21, float m22, float m23,
              float m30, float m31, float m32, float m33)
            : m00(m00), m01(m01), m02(m02), m03(m03),
              m10(m10), m11(m11), m12(m12), m13(m13),
              m20(m20), m21(m21), m22(m22), m23(m23),
              m30(m30), m31(m31), m32(m32), m33(m33) { }

    Matrix4x4(const Matrix4x4& A)
            : m00(A.m00), m01(A.m01), m02(A.m02), m03(A.m03),
              m10(A.m10), m11(A.m11), m12(A.m12), m13(A.m13),
              m20(A.m20), m21(A.m21), m22(A.m22), m23(A.m23),
              m30(A.m30), m31(A.m31), m32(A.m32), m33(A.m33) { }


    /**
    * Returns the 4x4 identity matrix.
    */
    static Matrix4x4 identity();

    /**
    * Returns matrix encoding a 3D counter-clockwise rotation by the angle theta in homogeneous coordinates. The angle is given in Radians.
    */
    static Matrix4x4 rotation(float theta, Axis axis);

    /**
    * Returns matrix encoding 3D translation by the vector t in homogeneous coordinates.
    */
    static Matrix4x4 translation(Vector3D t);

    /**
    * Returns matrix encoding 3D scaling by the vector s in homogeneous coordinates.
    */
    static Matrix4x4 scaling(Vector3D s);

    /**
    * Returns the determinant of m.
    */
    float det() const;

    /**
    * Returns the transpose of m.
    */
    Matrix4x4 T() const;

    /**
    * Returns the inverse of A.
    */
    Matrix4x4 inv() const;

    // accesses element (i,j) of A using 0-based indexing
          float& operator()(int i, int j);
    const float& operator()(int i, int j) const;

    // returns -m
    Matrix4x4 operator-() const;

    // returns m+A
    Matrix4x4 operator+(const Matrix4x4& A) const;

    // returns m-A
    Matrix4x4 operator-(const Matrix4x4& A) const;

    // returns m*f
    Matrix4x4 operator*(float f) const;

    // returns m*A
    Matrix4x4 operator*(const Matrix4x4& A) const;

    // returns m/f
    Matrix4x4 operator/(float f) const;

  private:
    union
    {
      float m[4][4];
      struct
      {
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;
      };
    };
  };

  // returns v*A
  Vector4D operator*(const Vector4D& v, const Matrix4x4& A);

  // returns v*A for a 3D vector v by using homogeneous coordinates
  Vector3D operator*(const Vector3D& v, const Matrix4x4& A);

  // returns f*A
  Matrix4x4 operator*(float f, const Matrix4x4& A);

  std::ostream& operator<<(std::ostream& os, const Matrix4x4& A);
}



#endif