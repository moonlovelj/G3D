#ifndef __COMMON_H__
#define __COMMON_H__

#include "Vector3D.h"

#include <cmath>
#include <limits>
#include <algorithm>

namespace g3dcommon
{

#define PI    (3.14159265358979323)
#define PI_F  (3.14159265358979f)
#define EPS_F (0.00001f)
#define INF_F (std::numeric_limits<float>::infinity())


  /* The unknown key */
#define G3D_KEY_UNKNOWN            -1

  /* Printable keys */
#define G3D_KEY_SPACE              32
#define G3D_KEY_APOSTROPHE         39  /* ' */
#define G3D_KEY_COMMA              44  /* , */
#define G3D_KEY_MINUS              45  /* - */
#define G3D_KEY_PERIOD             46  /* . */
#define G3D_KEY_SLASH              47  /* / */
#define G3D_KEY_0                  48
#define G3D_KEY_1                  49
#define G3D_KEY_2                  50
#define G3D_KEY_3                  51
#define G3D_KEY_4                  52
#define G3D_KEY_5                  53
#define G3D_KEY_6                  54
#define G3D_KEY_7                  55
#define G3D_KEY_8                  56
#define G3D_KEY_9                  57
#define G3D_KEY_SEMICOLON          59  /* ; */
#define G3D_KEY_EQUAL              61  /* = */
#define G3D_KEY_A                  65
#define G3D_KEY_B                  66
#define G3D_KEY_C                  67
#define G3D_KEY_D                  68
#define G3D_KEY_E                  69
#define G3D_KEY_F                  70
#define G3D_KEY_G                  71
#define G3D_KEY_H                  72
#define G3D_KEY_I                  73
#define G3D_KEY_J                  74
#define G3D_KEY_K                  75
#define G3D_KEY_L                  76
#define G3D_KEY_M                  77
#define G3D_KEY_N                  78
#define G3D_KEY_O                  79
#define G3D_KEY_P                  80
#define G3D_KEY_Q                  81
#define G3D_KEY_R                  82
#define G3D_KEY_S                  83
#define G3D_KEY_T                  84
#define G3D_KEY_U                  85
#define G3D_KEY_V                  86
#define G3D_KEY_W                  87
#define G3D_KEY_X                  88
#define G3D_KEY_Y                  89
#define G3D_KEY_Z                  90
#define G3D_KEY_a                  97
#define G3D_KEY_b                  98
#define G3D_KEY_c                  99
#define G3D_KEY_d                  100
#define G3D_KEY_e                  101
#define G3D_KEY_f                  102
#define G3D_KEY_g                  103
#define G3D_KEY_h                  104
#define G3D_KEY_i                  105
#define G3D_KEY_j                  106
#define G3D_KEY_k                  107
#define G3D_KEY_l                  108
#define G3D_KEY_m                  109
#define G3D_KEY_n                  110
#define G3D_KEY_o                  111
#define G3D_KEY_p                  112
#define G3D_KEY_q                  113
#define G3D_KEY_r                  114
#define G3D_KEY_s                  115
#define G3D_KEY_t                  116
#define G3D_KEY_u                  117
#define G3D_KEY_v                  118
#define G3D_KEY_w                  119
#define G3D_KEY_x                  120
#define G3D_KEY_y                  121
#define G3D_KEY_z                  122

  /*
  * Takes any kind of number and converts from degrees to radians.
  */
  template<typename T>
  inline T Radians(T deg)
  {
    return deg * (PI_F / 180);
  }

  /*
  * Takes any kind of number and converts from radians to degrees.
  */
  template<typename T>
  inline T Degrees(T rad)
  {
    return rad * (180 / PI_F);
  }

  /*
  * Takes any kind of number, as well as a lower and upper bound, and clamps the
  * number to be within the bound.
  */
  template<typename T>
  inline T Clamp(T x, T lo, T hi)
  {
    return std::min(std::max(x, lo), hi);
  }

  /**
  * Returns the distance from the plane to the point,
  * the plane is defined in the form of dot(p,n)=d (
  * p is any point on the plane, n is a normal of the
  * plane and must be a unit vector).
  *
  * The return value is a signed distance. A positive
  * number indicates the point on the front of the plane,
  * a negative number indicates the point on the back of
  * the plane, and 0 indicates the point on the plane.
  */
  inline float DistancePlaneToPoint(const Vector3D& n, const float d, const Vector3D& pos)
  {
    return Dot(pos, n) - d;
  }

}

#endif