#ifndef __COMMON_H__
#define __COMMON_H__

#define PI (3.14159265358979323)
#define EPS_F (0.00001f)
#define INF_F (std::numeric_limits<float>::infinity())

/*
Takes any kind of number and converts from degrees to radians.
*/
template<typename T>
inline T Radians(T deg)
{
  return deg * (PI / 180);
}

/*
Takes any kind of number and converts from radians to degrees.
*/
template<typename T>
inline T Degrees(T rad)
{
  return rad * (180 / PI);
}


#endif