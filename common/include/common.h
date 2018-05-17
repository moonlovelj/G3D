#ifndef __COMMON_H__
#define __COMMON_H__

#include <cmath>
#include <limits>
#include <algorithm>

#define PI    (3.14159265358979323)
#define PI_F  (3.14159265358979f)
#define EPS_F (0.00001f)
#define INF_F (std::numeric_limits<float>::infinity())

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

#endif