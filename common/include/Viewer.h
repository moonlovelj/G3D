#ifndef __VIEWER_H__
#define __VIEWER_H__

#include "Renderer.h"
#include <SDL.h>
#include <vector>
#include <chrono>

namespace g3dcommon
{
    /**
    * Provides SDL context, window display, and event handling routines.
    * A user application may draw to the window's context by providing
    * a user renderer. The viewer manages other display components such as the
    * zoom views, text OSD, etc. It also takes care of window event handling and
    * event passing, through which the renderer may interact with user inputs.
    */
    class Viewer
    {
    public:
        Viewer(size_t width = 960, size_t height = 640);
        ~Viewer();

        bool Init();

        void Start();

        void SetRenderer(Renderer *renderer);

    private:
        /**
        * Main update loop.
        */
        static void Update();

        /**
        * Process close window.
        */
        static void Close();

        // Quit window mark.
        static bool bQuit;

        // Window properties.
        static SDL_Window *window;
        static SDL_Surface *screenSurface;
        static size_t bufferWidth;
        static size_t bufferHeight;

        // Framebuffer for renderer.
        static std::vector<unsigned char> framebuffer;

        // Event handler
        static SDL_Event sdlEvent;

        // User space renderer
        static Renderer *renderer;

        // For compute fps.
        static int fps;
        static std::chrono::time_point<std::chrono::system_clock> sysLast;
        static std::chrono::time_point<std::chrono::system_clock> sysCurr;
    };
}


#endif