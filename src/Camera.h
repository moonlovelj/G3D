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

    // Convert the position in the world space to the position int the camera space.
    Vector3D ConvertWorldToCamera(const Vector3D& v);

    // Project the position in the camera space to the position on the viewing plane.
    Vector3D ProjectToView(const Vector3D& v);

    // Convert the position on the viewing plane to the position on the screen.
    // v must be the return value of the ProjectToView function.
    Vector3D ConvertViewToScreen(const Vector3D& v);

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
    // Directx style projection matrix, x and y are formatted between - 1 and 1,
    // z is formatted between 0 and 1.
    Matrix4x4 projectionMatrix;


  };

}


#endif