#ifndef __SCENEOBJECT_H__
#define __SCENEOBJECT_H__

#include "Vector3D.h"
#include "Matrix4x4.h"

namespace g3dcommon
{
  class Renderer;
  class Camera;

  /**
  * The base class of objects in the scene.
  */
  class SceneObject
  {
  public:
    SceneObject();
    virtual ~SceneObject();

    // Scene object rendering function.
    virtual void Render(Renderer* renderer) = 0;
    // Return index of the object.
    const size_t& Index() const { return index; }

    // Set world position.
    void SetPosition(const Vector3D& pos);
    // Returns world position.
    const Vector3D& GetPosition() const { return position; }
    // Set rotation around the x, y, z axis.
    void SetRotation(const Vector3D& r);
    // Returns rotation around the x, y, z axis.
    const Vector3D& GetRotation() const { return rotation; }
    // Set scale x, y, z
    void SetScale(const Vector3D& s);
    // Returns scale.
    const Vector3D& GetScale() const { return scale; }
    // Returns model to world matrix.
    const Matrix4x4& GetTransformationMatrix() const { return m2w; }

    // Set visibility of the scene object.
    void SetVisible(bool bVisible) { isVisible = bVisible; }
    // Returns visibility of the scene object.
    bool IsVisible() const { return isVisible; }
    // Returns the maximum radius of the object.
    const float& GetMaxRadius() const { return maxRadius; }
    // Set camera.
    void SetCamera(Camera* camera) { this->camera = camera; }
    // Returns camera.
    Camera* GetCamera() { return camera; }

   protected:
     // Calculate model to world matrix and update m2w.
     void CalculateModelToWorldMatrix();

     // Index of the object.
     static size_t kSceneObjectIndex;
     size_t index;

     // Visibility of the object.
     bool isVisible;
     // World-space position, rotation, scale.
     Vector3D position;
     Vector3D rotation;
     Vector3D scale;
     // Model to world matrix.
     Matrix4x4 m2w;
     // The maximum radius of the object.
     float maxRadius;
     // The camera used to render the object.
     Camera* camera;
  };
}

#endif