#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>

using namespace std;

struct Comparator
{
  SDL_FPoint p0;

  Comparator(SDL_FPoint ref) : p0(ref) {}

  static double squared_dist(SDL_FPoint &p1, SDL_FPoint &p2)
  {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
  }

  int orientation(SDL_FPoint &a, SDL_FPoint &b, SDL_FPoint &c)
  {
    int value = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    // Colinear
    if (value == 0)
      return 0;
    // CW or CCW, respectively
    return (value > 0) ? 1 : 2;
  };

  bool comp(SDL_FPoint &p0, SDL_FPoint &p1, SDL_FPoint &p2)
  {
    int ori = orientation(p0, p1, p2);
    if (ori == 0)
    {
      return (squared_dist(p0, p2) >= squared_dist(p0, p1)) ? false : true;
    }
    return (ori == 2) ? false : true;
  }

  bool operator()(SDL_FPoint &p1, SDL_FPoint &p2)
  {
    return comp(p0, p1, p2);
  }
};

class Geometry
{
private:
  static SDL_FPoint p0;
  static void out_point(SDL_FPoint &p);

public:
  vector<SDL_FPoint> points = {};
  vector<pair<int, int>> edges = {};
  Geometry();
  Geometry(vector<vector<SDL_FPoint>> &hulls, double eps = 1);
  void renderPoints(GLfloat size = 2);
  void renderEdges();
  void renderPolygon();
  void render();
};

#endif
