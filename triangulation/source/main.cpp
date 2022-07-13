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
#include "demo.h"
#include "polygon.h"
#include "display.h"

#define IMG_PATH "assets/Apollo_white.png"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

using namespace std;

int main(int args, char *argv[])
{

  if (args < 2)
  {
    cerr << "main() Error: OBJ filename not provided" << endl;
    return 1;
  }

  bool animate = false;
  bool write_out = false;
  int iter_count = 0;
  int point_try = 14;

  if (args >= 3 && strcmp(argv[2], "1") == 0)
  {
    write_out = true;
  }
  if (args >= 4)
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
  const string title = "Triangulation from Hull: " + base_filename;

  Display display(WINDOW_WIDTH, WINDOW_HEIGHT, title);

  vector<vector<Point>> object = load_obj(filename);
  vector<vector<Point>> test = joined_convex_hull(object);

  Geometry geo = Geometry(object);
  Demo demo = Demo(object);

  demo.triangulate(-1);

  geo.calc_hulls();
  geo.triangulate();

  if (write_out)
  {
    write_obj(geo, base_filename);
  }

  bool display_hull = !animate;
  bool display_triangles = !animate;

  int mTime = SDL_GetTicks(), cTime = SDL_GetTicks();

  while (!display.IsClosed())
  {
    if (animate)
    {
      cTime = SDL_GetTicks();
      if (cTime > mTime + 400)
      {
        iter_count++;
        demo.triangulate(iter_count);
        mTime = cTime;
      }
    }

    // cTime = SDL_GetTicks();
    // if (cTime > mTime + 1000)
    // {
    //   point_try--;
    //   cout << point_try << endl;
    //   mTime = cTime;
    // }

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
      glColor3f(1, 0.5, 0.3);
      // geo.renderHulls();
      geo.renderTriangles();
    }

    if (animate)
    {
      demo.render();
    }

    // geo.renderDebug(2, 5, 13);
    // geo.renderDebugEdge(9, 10);
    // geo.renderDebugEdge(10, 8);
    // geo.renderDebugEdge(8, 9);
    // geo.renderDebugEdge(9, 7);
    display.Update(&display_hull, &display_triangles);
  }

  return 0;
}
