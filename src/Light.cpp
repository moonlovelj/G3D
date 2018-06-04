#include "Light.h"
#include "Common.h"

namespace g3dcommon
{


  DirectionalLight::DirectionalLight(const Color& rad, const Vector3D& lightDir)
    : radiance(rad)
  {
    dirToLight = -lightDir.Unit();
  }

  Color DirectionalLight::SampleL(const Vector3D& p, Vector3D* wi) const
  {
    *wi = dirToLight;
    return radiance;
  }


  PointLight::PointLight(const Color& rad, const Vector3D& pos, float c /*= 0.f*/, float l /*= 0.002f*/, float q /*= 0.f*/)
    : radiance(rad),
    position(pos),
    kc(c),
    kl(l),
    kq(q)
  {

  }

  Color PointLight::SampleL(const Vector3D& p, Vector3D* wi) const
  {
    *wi = position - p;
    float d = wi->Norm();
    wi->Normalize();
    return radiance * (1.f / (kc + kl*d + kq*d*d));
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

  }

  Color SpotLight::SampleL(const Vector3D& p, Vector3D* wi) const
  {
    *wi = position - p;
    float d = wi->Norm();
    wi->Normalize();
    float cosTheta = Dot(direction, -(*wi));
    Color r(0.f, 0.f, 0.f, 1.f);
    if (cosTheta > 0.f && cosTheta > cos(Radians(angle * 0.5f)))
    {
      float eIndex = cosTheta;
      for (int i = 1; i < pf; i++)
      {
        eIndex *= cosTheta;
      }
      r = radiance * eIndex * (1.f / (kc + kl*d + kq*d*d));
    }
    return r;
  }

}