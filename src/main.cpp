#include "Viewer.h"
#include <iostream>
#include "SoftwareRenderer.h"
#include "Scene.h"
#include <vector>

int main(int argc, char **argv)
{
    g3dcommon::Viewer viewer(640, 480);
    if (viewer.Init())
    {
        std::vector<std::string> objs;
        if (argc > 1)
        {
            objs.resize(argc - 1);
            for (int i = 1; i < argc; ++i)
            {
                objs.emplace_back(argv[i]);
            }
        }
        else
        {
            objs.resize(1);
            objs.emplace_back("Crate1.obj");
        }

        g3dcommon::SoftwareRenderer *renderer = new g3dcommon::SoftwareRenderer;
        renderer->SetRenderScene(new g3dcommon::Scene(objs));
        viewer.SetRenderer(renderer);
        viewer.Start();
    }
    else
    {
        std::cout << "viewer Init failed !" << std::endl;
    }

    return 0;
}