#include "Camera.h"
#include "Common.h"
#include "SceneObject.h"

namespace g3dcommon
{

  Camera::Camera(const Vector3D& pos, const Vector3D& lookAt,
                 const Vector3D& up, float hFov, float nClip,
                 float fClip, size_t screenWidth, size_t screenHeight)
  {
    this->pos = pos;
    this->targetPos = lookAt;
    this->hFov = hFov;
    this->nClip = nClip;
    this->fClip = fClip;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    aspectRatio = static_cast<float>(screenWidth) / screenHeight;

    screenDist = 0.5f * 2 / tan(Radians(hFov)*0.5f);
    vFov = Degrees(2.f * atan(tan(Radians(hFov)*0.5f) / aspectRatio));

    // Camera z axis.
    Vector3D dir = (targetPos - pos).Unit();
    // Camera x axis.
    Vector3D right = Cross(up, dir).Unit();
    // Camera y axis.
    Vector3D upNew = Cross(dir, right);
    // World-to-camera translation.
    Vector3D transW2C(-pos);

    w2c = Matrix4x4(right.x,              upNew.x,              dir.x,              0,
                    right.y,              upNew.y,              dir.y,              0,
                    right.z,              upNew.z,              dir.z,              0,
                    Dot(transW2C, right), Dot(transW2C, upNew), Dot(transW2C, dir), 1);

    
    projectionMatrix(0, 0) = screenDist;
    projectionMatrix(1, 1) = screenDist*aspectRatio;
    projectionMatrix(2, 2) = fClip / (fClip - nClip);
    projectionMatrix(3, 2) = nClip*fClip / (nClip - fClip);
    projectionMatrix(2, 3) = 1.f;
  }

  Camera::Camera(const Camera& c)
  {
    pos = c.pos;
    targetPos = c.targetPos;
    hFov = c.hFov;
    vFov = c.vFov;
    nClip = c.nClip;
    fClip = c.fClip;
    screenWidth = c.screenWidth;
    screenHeight = c.screenHeight;
    screenDist = c.screenDist;
    aspectRatio = c.aspectRatio;
    w2c = c.w2c;
  }

  Camera::~Camera()
  {

  }

  Vector3D Camera::ConvertWorldToCamera(const Vector3D& v)
  {
    return (Vector4D(v.x, v.y, v.z, 1)*w2c).ProjectTo3D();
  }

  Vector3D Camera::ProjectToView(const Vector3D& v)
  {
    Vector4D vw(v.x, v.y, v.z, 1.f);
    return (vw * projectionMatrix).ProjectTo3D();
  }

  Vector3D Camera::ConvertViewToScreen(const Vector3D& v)
  {
    float x = (v.x + 1.f) * 0.5f * (screenWidth - 1.f);
    float y = (v.y + 1.f) * 0.5f * (screenHeight - 1.f);
    return Vector3D(x, y, v.z);
  }

  void Camera::SetFov(float fov)
  {
    hFov = fov;
    screenDist = 0.5f * 2 / tan(Radians(hFov)*0.5f);
    vFov = Degrees(2.f * atan(tan(Radians(hFov)*0.5f) / aspectRatio));
    projectionMatrix(0, 0) = screenDist;
    projectionMatrix(1, 1) = screenDist*aspectRatio;
  }

  void Camera::SetNearClip(float nClip)
  {
    this->nClip = nClip;
    projectionMatrix(2, 2) = fClip / (fClip - nClip);
    projectionMatrix(3, 2) = nClip*fClip / (nClip - fClip);
  }

  void Camera::SetFarClip(float fClip)
  {
    this->fClip = fClip;
    projectionMatrix(2, 2) = fClip / (fClip - nClip);
    projectionMatrix(3, 2) = nClip*fClip / (nClip - fClip);
  }

  void Camera::SetScreenSize(size_t width, size_t height)
  {
    screenWidth = width;
    screenHeight = height;
    aspectRatio = static_cast<float>(screenWidth) / screenHeight;
    vFov = Degrees(2.f * atan(tan(Radians(hFov)*0.5f) / aspectRatio));
    projectionMatrix(1, 1) = screenDist*aspectRatio;
  }

  void Camera::SetPosition(const Vector3D& position)
  {
    pos = position;

    Vector3D up = Up();
    // Camera z axis.
    Vector3D dir = (targetPos - pos).Unit();
    // Camera x axis.
    Vector3D right = Cross(up, dir).Unit();
    // Camera y axis.
    Vector3D upNew = Cross(dir, right);
    // World-to-camera translation.
    Vector3D transW2C(-pos);

    w2c(0, 0) = right.x;              w2c(0, 1) = upNew.x;              w2c(0, 2) = dir.x;
    w2c(1, 0) = right.y;              w2c(1, 1) = upNew.y;              w2c(1, 2) = dir.y;
    w2c(2, 0) = right.z;              w2c(2, 1) = upNew.z;              w2c(2, 2) = dir.z;
    w2c(3, 0) = Dot(transW2C, right); w2c(3, 1) = Dot(transW2C, upNew); w2c(3, 2) = Dot(transW2C, dir);

  }

  void Camera::SetTargetPosition(const Vector3D& lookAt)
  {
    targetPos = lookAt;

    Vector3D up = Up();
    // Camera z axis.
    Vector3D dir = (targetPos - pos).Unit();
    // Camera x axis.
    Vector3D right = Cross(up, dir).Unit();
    // Camera y axis.
    Vector3D upNew = Cross(dir, right);
    // World-to-camera translation.
    Vector3D transW2C(-pos);

    w2c(0, 0) = right.x;              w2c(0, 1) = upNew.x;              w2c(0, 2) = dir.x;
    w2c(1, 0) = right.y;              w2c(1, 1) = upNew.y;              w2c(1, 2) = dir.y;
    w2c(2, 0) = right.z;              w2c(2, 1) = upNew.z;              w2c(2, 2) = dir.z;
    w2c(3, 0) = Dot(transW2C, right); w2c(3, 1) = Dot(transW2C, upNew); w2c(3, 2) = Dot(transW2C, dir);
  }

  Vector3D Camera::Up() const
  {
    return Vector3D(w2c(0, 1), w2c(1, 1), w2c(2, 1));
  }

  Vector3D Camera::Dir() const
  {
    return Vector3D(w2c(0, 2), w2c(1, 2), w2c(2, 2));
  }

  Vector3D Camera::Right() const
  {
    return Vector3D(w2c(0, 0), w2c(1, 0), w2c(2, 0));
  }

  void Camera::MoveForward(float dist)
  {
    Vector3D up = Up();
    Vector3D dir = Dir();
    Vector3D right = Right();
    Vector3D moveVector = dir * dist;
    targetPos += moveVector;
    pos += moveVector;
    // World-to-camera translation.
    Vector3D transW2C(-pos);
    w2c(3, 0) = Dot(transW2C, right); w2c(3, 1) = Dot(transW2C, up); w2c(3, 2) = Dot(transW2C, dir);
  }

  void Camera::Move(const Vector3D& v)
  {
    Vector3D up = Up();
    Vector3D dir = Dir();
    Vector3D right = Right();
    targetPos += v;
    pos += v;
    // World-to-camera translation.
    Vector3D transW2C(-pos);
    w2c(3, 0) = Dot(transW2C, right); w2c(3, 1) = Dot(transW2C, up); w2c(3, 2) = Dot(transW2C, dir);
  }

  bool Camera::CullObject(SceneObject* object) const
  {
    Vector3D position = object->GetPosition() * w2c;
    float maxRadius = object->GetMaxRadius();
    if (position.z + maxRadius < nClip || position.z - maxRadius > fClip)
    {
      return true;
    }

    float halfH = tan(Radians(hFov*0.5f)) * fClip;
    float halfV = tan(Radians(vFov*0.5f)) * fClip;
    Vector3D leftTop(-halfH, halfV, fClip);
    Vector3D rightTop(halfH, halfV, fClip);
    Vector3D leftBottom(-halfH, -halfV, fClip);
    Vector3D rightBottom(halfH, -halfV, fClip);

    // test left plane.
    Vector3D n = Cross(leftBottom, leftTop);
    n.Normalize();
    if (DistancePlaneToPoint(n, 0.f, position) > maxRadius)
    {
      return true;
    }

    // test right plane.
    n = Cross(rightTop, rightBottom);
    n.Normalize();
    if (DistancePlaneToPoint(n, 0.f, position) > maxRadius)
    {
      return true;
    }

    // test top plane.
    n = Cross(leftTop, rightTop);
    n.Normalize();
    if (DistancePlaneToPoint(n, 0.f, position) > maxRadius)
    {
      return true;
    }

    // test bottom plane.
    n = Cross(rightBottom, leftBottom);
    n.Normalize();
    if (DistancePlaneToPoint(n, 0.f, position) > maxRadius)
    {
      return true;
    }

    return false;
  }

  bool Camera::CullFace(const Vector3D& n, const Vector3D& point) const
  {
    Vector3D view = pos - point;
    return Dot(n, view) < 0.f;
  }
  
}
