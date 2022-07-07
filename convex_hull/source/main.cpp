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

  const char *filename = argv[1];
  string path(filename);
  string base_filename = path.substr(path.find_last_of("/\\") + 1);
  const string title = "Convex Hull: " + base_filename;

  Display display(WINDOW_WIDTH, WINDOW_HEIGHT, title);

  vector<vector<SDL_FPoint>> object = load_obj(filename, WINDOW_WIDTH, WINDOW_HEIGHT);
  vector<vector<SDL_FPoint>> test = joined_convex_hull(object);

  Geometry geo = Geometry(test);

  while (!display.IsClosed())
  {
    display.Clear();

    glPointSize(4);
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    glVertex2f(-120, -60);
    glVertex2f(0.0, -60);
    glVertex2f(0.0, 60);
    glEnd();
    display.Update();
  }

  return 0;
}