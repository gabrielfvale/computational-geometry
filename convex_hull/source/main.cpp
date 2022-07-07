#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <tuple>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "objectio.h"
#include "convexhull.h"
#include "geometry.h"
#include "display.h"

#define IMG_PATH "assets/Apollo_white.png"

int main(int args, char *argv[])
{

  if (args < 2)
  {
    std::cerr << "main() Error: OBJ filename not provided" << std::endl;
    return 1;
  }

  const char *filename = argv[1];
  std::string path(filename);
  std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
  const std::string title = "Convex Hull: " + base_filename;

  Display display(640, 480, title);

  while (!display.IsClosed())
  {
    display.Clear();
    display.Update();
  }

  return 0;
}
