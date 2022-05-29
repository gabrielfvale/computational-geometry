#include <iostream>
#include <SDL2/SDL.h>

int main(int args, char *argv[])
{

  const int windowWidth = 640;
  const int windowHeight = 480;

  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cerr << "SDL_Init() Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  // Don't stop window compositor
  SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

  SDL_Window *window;
  SDL_Renderer *renderer;

  // Create window
  window = SDL_CreateWindow(
      "Convex Hull",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      windowWidth, windowHeight,
      SDL_WINDOW_SHOWN);
  if (window == nullptr)
  {
    std::cerr << "SDL_CreateWindow() Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  // Create renderer
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == nullptr)
  {
    std::cerr << "SDL_CreateRenderer() Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    return 1;
  }

  const int pointLocationx = windowWidth / 2;
  const int pointLocationy = windowHeight / 2;

  SDL_Event event;
  bool quit = false;

  while (!quit)
  {
    while (SDL_PollEvent(&event) != 0)
    {
      if (event.type == SDL_QUIT)
      {
        quit = true;
      }
    }

    // Clear the screen
    SDL_RenderClear(renderer);

    // Set color of renderer
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderDrawPoint(renderer, pointLocationx, pointLocationy);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Show all the has been done behind the scenes
    SDL_RenderPresent(renderer);
  }

  // Clean Up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
