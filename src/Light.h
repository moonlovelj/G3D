#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Color.h"
#include "Vector3D.h"

namespace g3dcommon
{
  class SceneLight
  {
  public:
    virtual Color SampleL(const Vector3D& p, Vector3D* wi) const = 0;
  };


  class DirectionalLight : public SceneLight
  {
  public:
    DirectionalLight(const Color& rad, const Vector3D& lightDir);
    Color SampleL(const Vector3D& p, Vector3D* wi) const;
  private:
    Color radiance;
    Vector3D dirToLight;
  };

  class PointLight : public SceneLight
  {
  public:
    PointLight(const Color& rad, const Vector3D& pos, float c = 0.f, float l = 0.002f, float q = 0.f);
    Color SampleL(const Vector3D& p, Vector3D* wi) const;
  private:
    Color radiance;
    Vector3D position;
    // Constant attenuation factor, linear attenuation factor, secondary attenuation factor.
    float kc, kl, kq;
  };

  class SpotLight : public SceneLight
  {
  public:
    SpotLight(const Color& rad, const Vector3D& pos,const Vector3D& dir, 
      float angle, float c = 0.f, float l = 0.002f, float q = 0.f, int p = 2);

    Color SampleL(const Vector3D& p, Vector3D* wi) const;

  private:
    Color radiance;
    Vector3D position;
    Vector3D direction;
    float angle;
    // Constant attenuation factor, linear attenuation factor, secondary attenuation factor.
    float kc, kl, kq;
    // Exponent factor.
    int pf;
  }; 


}
#endif