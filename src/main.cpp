#include "Viewer.h"
#include <iostream>
#include "SoftwareRenderer.h"
int main(int argc, char** argv)
{
  g3dcommon::Viewer viewer;
  if (viewer.Init())
  {
    g3dcommon::SoftwareRenderer* renderer = new g3dcommon::SoftwareRenderer;
    renderer->Init();
    viewer.SetRenderer(renderer);
    viewer.Start();
  }
  else
  {
    std::cout << "viewer Init failed !" << std::endl;
  }

  return 0;
}