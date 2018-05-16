#include "Viewer.h"
#include <iostream>
namespace g3dcommon
{

  bool Viewer::bQuit = false;
  SDL_Window* Viewer::window = nullptr;
  SDL_Surface* Viewer::screenSurface = nullptr;
  SDL_Surface* Viewer::renderSurface = nullptr;
  size_t Viewer::bufferWidth = 960;
  size_t Viewer::bufferHeight = 640;
  Renderer* Viewer::renderer = nullptr;
  std::vector<unsigned char> Viewer::framebuffer;
  SDL_Event Viewer::sdlEvent;


  Viewer::Viewer(size_t width, size_t height)
  {
    bufferWidth = width;
    bufferHeight = height;
    framebuffer.resize(4 * width * height);
  }

  Viewer::~Viewer()
  {

  }

  bool Viewer::Init()
  {
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      std::cout << "SDL could not initialize! SDL_Error:" << SDL_GetError() << std::endl;
      success = false;
    }
    else
    {
      // Create window
      window = SDL_CreateWindow("G3D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, bufferWidth, bufferHeight, SDL_WINDOW_SHOWN);
      if (window == nullptr)
      {
        std::cout << "Window could not be created! SDL_Error:" << SDL_GetError() << std::endl;
        success = false;
      }
      else
      {
        // Get window surface
        screenSurface = SDL_GetWindowSurface(window);
        // Create render surface
        renderSurface = SDL_CreateRGBSurface(0, bufferWidth, bufferHeight, 32, 0, 0, 0, 0);
        if (renderSurface == NULL)
        {
          std::cout << "Pixel surface could not be created! SDL_Error:" << SDL_GetError() << std::endl;
          success = false;
        }
      }
    }

    return success;
  }

  void Viewer::Start()
  {
    while (!bQuit)
    {
      Update();
    }
    Close();
  }

  void Viewer::SetRenderer(Renderer* renderer)
  {
    this->renderer = renderer;
    if (nullptr != this->renderer)
    {
      this->renderer->SetRenderTarget(&framebuffer[0], bufferWidth, bufferHeight);
    }
  }

  void Viewer::Update()
  {
    // Handle events on queue
    while (SDL_PollEvent(&sdlEvent) != 0)
    {
      // User requests quit
      if (sdlEvent.type == SDL_QUIT)
      {
        bQuit = true;
      }
      else if (sdlEvent.type == SDL_KEYDOWN)
      {
        switch (sdlEvent.key.keysym.sym)
        {
        case SDLK_ESCAPE:
          bQuit = true;
          break;
        default:
          break;
        }
      }
    }

    // Lock surface.
    SDL_LockSurface(screenSurface);
    // Render framebuffer by renderer.
    if (nullptr != renderer)
    {
      renderer->Render();
    }
    memcpy((unsigned char*)renderSurface->pixels, &framebuffer[0], bufferWidth*bufferHeight);

    // Apply the pixel.
    SDL_BlitSurface(renderSurface, NULL, screenSurface, NULL);
    // Unlock surface.
    SDL_UnlockSurface(screenSurface);
    // Update the window.
    SDL_UpdateWindowSurface(window);
  }

  void Viewer::Close()
  {
    //Deallocate surface
    SDL_FreeSurface(screenSurface);
    screenSurface = nullptr;

    //Destroy window
    SDL_DestroyWindow(window);
    window = nullptr;

    //Quit SDL subsystems
    SDL_Quit();
  }

}

