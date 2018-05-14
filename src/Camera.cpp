#include "Camera.h"
#include "Common.h"

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

    screenDist = 0.5f * screenWidth / tan(Radians(hFov)*0.5f);
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
  }

  Camera::~Camera()
  {

  }

}
