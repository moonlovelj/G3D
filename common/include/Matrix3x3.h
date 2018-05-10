#ifndef __MATRIX3X3_H__
#define __MATRIX3X3_H__

#include "Vector3D.h"
#include <ostream>
namespace g3dcommon
{
  class Matrix3x3
  {
  public:
    Matrix3x3():m00(0.f), m01(0.f), m02(0.f),
                m10(0.f), m11(0.f), m12(0.f),
                m20(0.f), m21(0.f), m22(0.f) { }
    
    Matrix3x3(float m00, float m01, float m02,
              float m10, float m11, float m12, 
              float m20, float m21, float m22)
            : m00(m00), m01(m01), m02(m02),
              m10(m10), m11(m11), m12(m12), 
              m20(m20), m21(m21), m22(m22) { }

    Matrix3x3(const Matrix3x3& A)
            : m00(A.m00), m01(A.m01), m02(A.m02),
              m10(A.m10), m11(A.m11), m12(A.m12),
              m20(A.m20), m21(A.m21), m22(A.m22) { }


    /**
    * Returns the 3x3 identity matrix.
    */
    static Matrix3x3 Identity();

    /**
    * Returns the determinant of m.
    */
    float Det() const;

    /**
    * Returns the transpose of m.
    */
    Matrix3x3 T() const;

    /**
    * Returns the inverse of A.
    */
    Matrix3x3 Inv() const;

    // accesses element (i,j) of A using 0-based indexing
          float& operator()(int i, int j);
    const float& operator()(int i, int j) const;

    // returns -m
    Matrix3x3 operator-() const;

    // returns m+A
    Matrix3x3 operator+(const Matrix3x3& A) const;

    // returns m-A
    Matrix3x3 operator-(const Matrix3x3& A) const;

    // returns m*f
    Matrix3x3 operator*(float f) const;

    // returns m*A
    Matrix3x3 operator*(const Matrix3x3& A) const;

    // returns m/f
    Matrix3x3 operator/(float f) const;

  private:
    union
    {
      float m[3][3];
      struct
      {
        float m00, m01, m02;
        float m10, m11, m12;
        float m20, m21, m22;
      };
    };
  };

  // returns v*A
  Vector3D operator*(const Vector3D& v, const Matrix3x3& A);

  // returns f*A
  Matrix3x3 operator*(float f, const Matrix3x3& A);

  std::ostream& operator<<(std::ostream& os, const Matrix3x3& A);
}



#endif