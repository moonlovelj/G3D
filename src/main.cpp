
#include <stdio.h>
#include <iostream>
#include <SDL.h>


const short SCREEN_WIDTH = 1024;
const short SCREEN_HEIGHT = 768;

// The window we'll be rendering to
SDL_Window* gWindow = NULL;
// The surface contained by the window
SDL_Surface* gScreenSurface = NULL;
// Main loop flag
bool quit = false;
// Event handler
SDL_Event e;
// Screen pixel surface
SDL_Surface* gScreenPixelSurface = NULL;

// Init window
bool Init()
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
    gWindow = SDL_CreateWindow("G3D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
      std::cout << "Window could not be created! SDL_Error:" << SDL_GetError() << std::endl;
      success = false;
    }
    else
    {
      // Get window surface
      gScreenSurface = SDL_GetWindowSurface(gWindow);
      // Create screen pixel surface
      gScreenPixelSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
      if (gScreenPixelSurface == NULL)
      {
        std::cout << "Pixel surface could not be created! SDL_Error:" << SDL_GetError() << std::endl;
        success = false;
      }
    }
  }

  return success;
}

// Close window
void Close()
{
  //Deallocate surface
  SDL_FreeSurface(gScreenSurface);
  gScreenSurface = NULL;

  SDL_FreeSurface(gScreenPixelSurface);
  gScreenPixelSurface = NULL;

  //Destroy window
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  //Quit SDL subsystems
  SDL_Quit();
}

int main(int argc, char** argv)
{
  Init();
  // While application is running
  while (!quit)
  {
    // Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
      // User requests quit
      if (e.type == SDL_QUIT)
      {
        quit = true;
      }
      else if (e.type == SDL_KEYDOWN)
      {
        switch (e.key.keysym.sym)
        {
        case SDLK_ESCAPE:
          quit = true;
          break;
        default:
          break;
        }
      }
    }
    // Apply the image
    SDL_BlitSurface(gScreenPixelSurface, NULL, gScreenSurface, NULL);
    // Update the surface
    SDL_UpdateWindowSurface(gWindow);
  }
  Close();
  return 0;
}