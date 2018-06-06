#include "Light.h"
#include "Common.h"

namespace g3dcommon
{


  DirectionalLight::DirectionalLight(const Color& rad, const Vector3D& lightDir)
    : radiance(rad)
  {
    dirToLight = -lightDir.Unit();
  }

  Color DirectionalLight::SampleL(const Vector3D& p, const Vector3D& n) const
  {
    float cosTheta = Dot(n, dirToLight);
    return radiance * (cosTheta > 0.f ? cosTheta : 0.f);
  }


  PointLight::PointLight(const Color& rad, const Vector3D& pos, float c /*= 0.f*/, float l /*= 0.002f*/, float q /*= 0.f*/)
    : radiance(rad),
    position(pos),
    kc(c),
    kl(l),
    kq(q)
  {

  }

  Color PointLight::SampleL(const Vector3D& p, const Vector3D& n) const
  {
    Vector3D wi = position - p;
    float d = wi.Norm();
    wi.Normalize();
    float cosTheta = Dot(n, wi);
    return radiance * (1.f / (kc + kl*d + kq*d*d)) * (cosTheta > 0.f ? cosTheta : 0.f);
  }


  SpotLight::SpotLight(const Color& rad, const Vector3D& pos, const Vector3D& dir,
    float angle, float c /*= 0.f*/, float l /*= 0.002f*/, float q /*= 0.f*/, int p /*= 2*/)
    : radiance(rad),
    position(pos),
    direction(dir),
    angle(angle),
    kc(c),
    kl(l),
    kq(q),
    pf(2)
  {
    direction.Normalize();
  }

  Color SpotLight::SampleL(const Vector3D& p, const Vector3D& n) const
  {
    Vector3D wi = position - p;
    float d = wi.Norm();
    wi.Normalize();
    float cosAlpha = Dot(direction, -wi);

    if (cosAlpha < cos(Radians(angle * 0.5f)))
    {
      return Color(0.f, 0.f, 0.f, 1.f);
    }

    float eIndex = cosAlpha;
    for (int i = 1; i < pf; i++)
    {
      eIndex *= cosAlpha;
    }
    float cosTheta = Dot(n, wi);
    return radiance * eIndex * (1.f / (kc + kl*d + kq*d*d)) * (cosTheta > 0.f ? cosTheta : 0.f);
  }

}