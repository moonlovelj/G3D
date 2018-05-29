#include "SceneObject.h"
#include "Common.h"

namespace g3dcommon
{
  size_t SceneObject::kSceneObjectIndex = 0;

  SceneObject::SceneObject() :
    position(0.f, 0.f, 0.f),
    rotation(0.f, 0.f, 0.f),
    scale(1.f, 1.f, 1.f)
  {
    index = kSceneObjectIndex;
    ++kSceneObjectIndex;
    CalculateModelToWorldMatrix();
  }

  SceneObject::~SceneObject()
  {

  }

  void SceneObject::SetPosition(const Vector3D& pos)
  {
    position = pos;
    CalculateModelToWorldMatrix();
  }

  void SceneObject::SetRotation(const Vector3D& r)
  {
    rotation = r;
    CalculateModelToWorldMatrix();
  }

  void SceneObject::SetScale(const Vector3D& s)
  {
    scale = s;
    CalculateModelToWorldMatrix();
  }

  void SceneObject::CalculateModelToWorldMatrix()
  {
    m2w = Matrix4x4::Scaling(scale);
    m2w = m2w * Matrix4x4::Rotation(Radians(rotation.z), Matrix4x4::Axis::Z)
              * Matrix4x4::Rotation(Radians(rotation.y), Matrix4x4::Axis::Y)
              * Matrix4x4::Rotation(Radians(rotation.x), Matrix4x4::Axis::X);
    m2w = m2w * Matrix4x4::Translation(position);
  }

}