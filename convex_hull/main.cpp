#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <tuple>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "OBJLoader.h"
#include "ConvexHull.h"

#define IMG_PATH "objects/Apollo_white.png"

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

  if (TTF_Init() < 0)
  {
    std::cerr << "TTF_Init() Error: " << TTF_GetError() << std::endl;
  }

  // Don't stop window compositor
  SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *img = NULL;
  TTF_Font *font;
  int iw, ih;

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

  // Load image
  img = IMG_LoadTexture(renderer, IMG_PATH);
  SDL_QueryTexture(img, NULL, NULL, &iw, &ih); // get the width and height of the texture
  SDL_Rect texr;
  texr.x = 0;
  texr.y = 0;
  texr.w = iw;
  texr.h = ih;

  // Load font
  font = TTF_OpenFont("fonts/OpenSans-Regular.ttf", 16);
  SDL_Surface *text1;
  SDL_Surface *text2;

  SDL_FPoint *points;
  int size;
  std::vector<std::vector<int>> faces;
  std::tie(points, size) = load_obj(filename, windowWidth, windowHeight, faces);

  SDL_FPoint *hull_points;
  int hull_size;
  std::tie(hull_points, hull_size) = convex_hull(points, size);

  std::cout << "Loaded: " << filename << std::endl;
  std::cout << "Points: " << size << std::endl;
  std::cout << "Convex Hull Points: " << hull_size << std::endl;

  std::ostringstream oss;
  oss << "Points: " << size;
  std::string top_text = oss.str();
  oss.str("");
  oss << "Convex Hull: " << hull_size;
  std::string bottom_text = oss.str();

  // Create text to display
  text1 = TTF_RenderText_Solid(font, top_text.c_str(), {255, 255, 255});
  text2 = TTF_RenderText_Solid(font, bottom_text.c_str(), {255, 255, 255});
  SDL_Texture *text1_texture;
  SDL_Texture *text2_texture;

  text1_texture = SDL_CreateTextureFromSurface(renderer, text1);
  text2_texture = SDL_CreateTextureFromSurface(renderer, text2);

  SDL_Rect text1_dest = {0, 0, text1->w, text1->h};
  SDL_Rect text2_dest = {0, text1->h, text2->w, text2->h};

  SDL_Event event;
  bool quit = false;
  bool display_hull = false;
  bool display_wireframe = false;
  bool display_image = true;

  while (!quit)
  {
    while (SDL_PollEvent(&event) != 0)
    {
      switch (event.type)
      {
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_c:
          display_hull = !display_hull;
          break;
        case SDLK_w:
          display_wireframe = !display_wireframe;
          break;
        case SDLK_i:
          display_image = !display_image;
          break;
        }
        break;
      default:
        break;
      }
    }

    // Clear the screen
    SDL_RenderClear(renderer);

    if (display_image && strcmp(filename, "objects/Apollo.obj") == 0)
    {
      SDL_SetTextureAlphaMod(img, 32);
      SDL_RenderCopy(renderer, img, NULL, &texr);
    }

    SDL_RenderCopy(renderer, text1_texture, NULL, &text1_dest);
    SDL_RenderCopy(renderer, text2_texture, NULL, &text2_dest);

    // Set color of renderer
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Draw mesh points
    // SDL_RenderDrawPointsF(renderer, points, size);
    for (int i = 0; i < size; ++i)
    {
      // SDL_RenderDrawPointF(renderer, points[i].x, points[i].y);

      // Render extra points around for visibility
      // Top
      SDL_RenderDrawPointF(renderer, points[i].x - 1, points[i].y - 1);
      SDL_RenderDrawPointF(renderer, points[i].x, points[i].y - 1);
      SDL_RenderDrawPointF(renderer, points[i].x + 1, points[i].y - 1);
      // Bottom
      SDL_RenderDrawPointF(renderer, points[i].x - 1, points[i].y + 1);
      SDL_RenderDrawPointF(renderer, points[i].x, points[i].y + 1);
      SDL_RenderDrawPointF(renderer, points[i].x + 1, points[i].y + 1);
      // Sides
      SDL_RenderDrawPointF(renderer, points[i].x - 1, points[i].y);
      SDL_RenderDrawPointF(renderer, points[i].x + 1, points[i].y);
    }

    if (display_hull && hull_size > 0)
    {
      SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
      SDL_RenderDrawPointsF(renderer, hull_points, hull_size);

      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      for (int i = 0; i < hull_size - 1; ++i)
      {
        SDL_RenderDrawLine(renderer, hull_points[i].x, hull_points[i].y, hull_points[i + 1].x, hull_points[i + 1].y);
      }
      SDL_RenderDrawLine(renderer, hull_points[hull_size - 1].x, hull_points[hull_size - 1].y, hull_points[0].x, hull_points[0].y);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderPresent(renderer);
  }

  // Clean Up
  SDL_FreeSurface(text1);
  SDL_FreeSurface(text2);
  SDL_DestroyTexture(text1_texture);
  SDL_DestroyTexture(text2_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
