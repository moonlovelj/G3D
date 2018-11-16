#ifndef __VECOTR2D_H__
#define __VECOTR2D_H__

#include <ostream>
#include <cmath>

namespace g3dcommon
{

    /**
     * Defines 2D vectors.
     */
    class Vector2D
    {
    public:
        union
        {
            float m[2];
            struct
            {
                float x, y;
            };
        };

        Vector2D()
            : x(0.f), y(0.f)
        {}

        Vector2D(float x, float y)
            : x(x), y(y)
        {}

        Vector2D(const Vector2D &v)
            : x(v.x), y(v.y)
        {}

        inline Vector2D operator-() const
        {
            return Vector2D(-x, -y);
        }

        inline Vector2D operator+(const Vector2D &v) const
        {
            return Vector2D(x + v.x, y + v.y);
        }

        inline Vector2D operator-(const Vector2D &v) const
        {
            return Vector2D(x - v.x, y - v.y);
        }

        inline Vector2D operator*(float f) const
        {
            return Vector2D(x * f, y * f);
        }

        inline Vector2D operator/(float f) const
        {
            return Vector2D(x / f, y / f);
        }

        inline void operator+=(const Vector2D &v)
        {
            x += v.x;
            y += v.y;
        }

        inline void operator-=(const Vector2D &v)
        {
            x -= v.x;
            y -= v.y;
        }

        inline void operator*=(float f)
        {
            x *= f;
            y *= f;
        }

        inline void operator/=(float f)
        {
            x /= f;
            y /= f;
        }

        inline float Norm() const
        {
            return sqrt(x * x + y * y);
        }

        inline float Norm2() const
        {
            return x * x + y * y;
        }

        inline Vector2D Unit() const
        {
            return *this / this->Norm();
        }

        inline void Normalize()
        {
            *this /= this->Norm();
        }
    };

    inline Vector2D operator*(float f, const Vector2D &v)
    {
        return v * f;
    }

    inline float Dot(const Vector2D &v1, const Vector2D &v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    inline float Cross(const Vector2D &v1, const Vector2D &v2)
    {
        return v1.x * v2.y - v2.x * v1.y;
    }

}


#endif