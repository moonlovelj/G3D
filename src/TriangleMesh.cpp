#include "TriangleMesh.h"
#include "SoftwareRenderer.h"

namespace g3dcommon
{


  TriangleMesh::TriangleMesh()
  {

  }

  TriangleMesh::~TriangleMesh()
  {

  }

  void TriangleMesh::Load(const std::string& fileName)
  {

  }

  void TriangleMesh::Render(Renderer* renderer)
  {
    if (nullptr == renderer)
    {
      return;
    }
    for (auto & triangle : triangleList)
    {
      dynamic_cast<SoftwareRenderer*>(renderer)->DrawTriangle(triangle);
    }
  }

 

}