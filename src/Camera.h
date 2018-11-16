#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vector3D.h"
#include "Matrix4x4.h"

namespace g3dcommon
{
    class SceneObject;

    class Camera
    {
    public:
        Camera(const Vector3D &pos, const Vector3D &lookAt,
               const Vector3D &up, float hFov, float nClip,
               float fClip, size_t screenWidth, size_t screenHeight);

        Camera(const Camera &c);

        ~Camera();

        // Convert the position in the world space to the position int the camera space.
        Vector3D ConvertWorldToCamera(const Vector3D &v);

        // Project the position in the camera space to the position on the viewing plane.
        Vector3D ProjectToView(const Vector3D &v);

        // Convert the position on the viewing plane to the position on the screen.
        // v must be the return value of the ProjectToView function.
        Vector3D ConvertViewToScreen(const Vector3D &v);

        // Set horizontal field of view.
        void SetFov(float fov);
        // Returns the value of horizontal field of view.
        const float &GetFov() const
        { return hFov; }
        // Set near clipping plane.
        void SetNearClip(float nClip);
        // Returns the value of near clipping plane.
        const float &GetNearClip() const
        { return nClip; }
        // Set far clipping plane.
        void SetFarClip(float fClip);
        // Returns the value of far clipping plane.
        const float &GetFarClip() const
        { return fClip; }
        // Set screen size.
        void SetScreenSize(size_t width, size_t height);
        // Set position of eye.
        void SetPosition(const Vector3D &position);
        // Returns the position of eye.
        const Vector3D &GetPosition() const
        { return pos; }
        // Set target position.
        void SetTargetPosition(const Vector3D &lookAt);
        // Returns target position.
        const Vector3D &GetTargetPosition() const
        { return targetPos; }
        // Returns up vector.
        Vector3D Up() const;
        // Returns dir vector.
        Vector3D Dir() const;
        // Returns right vector.
        Vector3D Right() const;

        // Move the dist distance along the viewing axis.
        void MoveForward(float dist);
        // Move along v.
        void Move(const Vector3D &v);
        // Reject culling based on their maximum radius,
        // returns true if object is culled.
        bool CullObject(SceneObject *object) const;
        // Back face culling based on normal of face and any point on face,
        // returns true if face is culled,
        // parameter n does not need to be normalized.
        bool CullFace(const Vector3D &n, const Vector3D &point) const;

        // dTheta represents rotation about the up axis,
        // dPhi represents rotation about the right axis.
        void RotateBy(const float dTheta, const float dPhi);

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