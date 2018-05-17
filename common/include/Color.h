#ifndef __COLOR_H__
#define __COLOR_H__

#include <ostream>
#include <algorithm>

namespace g3dcommon
{
  /**
  * Encodes a color via additive red, green, and blue chanel values.
  * Each color chanel value is in the range [0,1]. The alpha value
  * defines the transparency of the color and is also in [0,1].
  */
  class Color
  {
  public:
    float r, g, b, a;

    Color(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f)
      : r(r), g(g), b(b), a(a) { }

    Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
    {
      float inv = 1.f / 255.f;
      r = inv * R;
      g = inv * G;
      b = inv * B;
      a = inv * A;
    }

    Color(const Color& c) : r(c.r), g(c.g), b(c.b), a(c.a) { }

    inline Color operator+(const Color& c) const
    {
      return Color(r + c.r, g + c.g, b + c.b, a + c.a);
    }

    inline Color operator-(const Color& c) const
    {
      return Color(r - c.r, g - c.g, b - c.b, a - c.a);
    }

    inline Color& operator+=(const Color& c)
    {
      r += c.r;
      g += c.g;
      b += c.b;
      a += c.a;
      return *this;
    }

    inline Color& operator-=(const Color& c)
    {
      r -= c.r;
      g -= c.g;
      b -= c.b;
      a -= c.a;
      return *this;
    }

    inline Color operator*(const Color& c) const
    {
      return Color(r*c.r, g*c.g, b*c.b, a*c.a);
    }

    inline Color& operator*=(const Color& c)
    {
      r *= c.r;
      g *= c.g;
      b *= c.b;
      a *= c.a;
      return *this;
    }

    inline Color operator*(float f) const
    {
      return Color(r*f, g*f, b*f, a*f);
    }

    inline Color& operator*=(float f)
    {
      r *= f;
      g *= f;
      b *= f;
      a *= f;
      return *this;
    }

    // Get the value of the red channel(unsigned character 0-255).
    inline unsigned char R() const
    {
      return std::min(std::max(static_cast<int>(r * 255), 0), 255);
    }
    // Get the value of the green channel(unsigned character 0-255).
    inline unsigned char G() const
    {
      return std::min(std::max(static_cast<int>(g * 255), 0), 255);
    }
    // Get the value of the blue channel(unsigned character 0-255).
    inline unsigned char B() const
    {
      return std::min(std::max(static_cast<int>(b * 255), 0), 255);
    }
    // Get the value of the alpha channel(unsigned character 0-255).
    inline unsigned char A() const
    {
      return std::min(std::max(static_cast<int>(a * 255), 0), 255);
    }
    // Formats four color channels to a range of 0 to 1.
    void Clamp();
    // Get values this convert color to 32bit unsigned integers(values between 0 and 255 for each color channel).
    unsigned int toUint32() const;
  };

  inline Color operator*(float f, const Color& c)
  {
    return c * f;
  }

  std::ostream& operator<<(std::ostream& os, const Color& c);
}

#endif