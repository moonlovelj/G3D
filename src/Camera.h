#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vector3D.h"
#include "Matrix4x4.h"

namespace g3dcommon
{
  class Camera
  {
  public:
    Camera(const Vector3D& pos, const Vector3D& lookAt, 
           const Vector3D& up, float hFov, float nClip, 
           float fClip,size_t screenWidth, size_t screenHeight);

    Camera(const Camera& c);

    ~Camera();

  private:
    // Horizontal and vertical field of view.
    float hFov, vFov;
    // Near clipping plane and far clipping plane.
    float nClip, fClip;
    // Aspect ratio of the screen.
    float aspectRatio;
    // The current position and the position that the camera is looking at.
    Vector3D pos, targetPos;
    // The width and height of the screen.
    size_t screenWidth, screenHeight;
    // Projection plane distance.
    float screenDist;
    // World-to-camera matrix.
    Matrix4x4 w2c;


  };

}


#endif