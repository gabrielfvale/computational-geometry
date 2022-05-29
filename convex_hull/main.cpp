#include <iostream>
#include <SDL2/SDL.h>
#include <tuple>

#include "OBJLoader.h"

int main(int args, char *argv[])
{

  if (args < 2)
  {
    std::cerr << "main() Error: OBJ filename not provided" << std::endl;
    return 1;
  }

  const char *filename = argv[1];
  const std::string title = "Convex Hull: " + std::string(filename);

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
      title.c_str(),
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

  SDL_FPoint *points;
  int size;
  std::tie(points, size) = loadOBJ(filename, windowWidth, windowHeight);

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

    SDL_RenderDrawPointsF(renderer, points, size);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < size - 1; ++i)
    {
      SDL_RenderDrawLine(renderer, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
    }
    SDL_RenderDrawLine(renderer, points[size - 1].x, points[size - 1].y, points[0].x, points[0].y);

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
