#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <tuple>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "point.h"
#include "objectio.h"
#include "convexhull.h"
#include "geometry.h"
#include "polygon.h"
#include "display.h"

#define IMG_PATH "assets/Apollo_white.png"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

using namespace std;

int main(int args, char *argv[])
{

  if (args < 2)
  {
    cerr << "main() Error: OBJ filename not provided" << endl;
    return 1;
  }

  bool animate = false;
  int iter_count = 0;
  if (args >= 3)
  {
    animate = true;
  }
  else
  {
    iter_count = -1;
  }

  const char *filename = argv[1];
  string path(filename);
  string base_filename = path.substr(path.find_last_of("/\\") + 1);
  const string title = "Convex Hull: " + base_filename;

  Display display(WINDOW_WIDTH, WINDOW_HEIGHT, title);

  vector<vector<Point>> object = load_obj(filename);
  vector<vector<Point>> test = joined_convex_hull(object);

  Geometry geo = Geometry(object);

  geo.calc_hulls();
  geo.triangulate(iter_count);

  bool display_hull = true;
  bool display_triangles = true;

  int mTime = SDL_GetTicks(), cTime = SDL_GetTicks();

  while (!display.IsClosed())
  {
    if (animate)
    {
      cTime = SDL_GetTicks();
      if (cTime > mTime + 200)
      {
        iter_count += 1;
        geo.triangulate(iter_count);
        mTime = cTime;
      }
    }

    display.Clear();
    glColor3f(1, 0, 0);
    geo.renderPoints(4);

    if (display_hull)
    {
      glColor3f(1, 1, 1);
      geo.renderHulls();
    }

    if (display_triangles)
    {
      glColor3f(1, 1, 0);
      geo.renderTriangles();
    }

    // geo.renderDebugEdge(6, 7);
    // geo.renderDebugEdge(7, 3);
    // geo.renderDebugEdge(6, 3);
    // geo.renderDebugEdge(3, 5);
    // geo.renderDebugEdge(0, 15);
    display.Update(&display_hull, &display_triangles);
  }

  return 0;
}
