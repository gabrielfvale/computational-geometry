#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>

using namespace std;

class Geometry
{
private:
  static void out_point(SDL_FPoint &p);
  static double squared_dist(SDL_FPoint &p1, SDL_FPoint &p2);
  static int orientation(SDL_FPoint &a, SDL_FPoint &b, SDL_FPoint &c);
  static bool comp(SDL_FPoint &p0, SDL_FPoint &p1, SDL_FPoint &p2);

public:
  vector<SDL_FPoint> points = {};
  vector<pair<int, int>> edges = {};
  Geometry();
  Geometry(vector<vector<SDL_FPoint>> &hulls, double eps = 1);
  void renderPoints(GLfloat size = 2);
  void renderEdges();
  void renderPolygon();
};

#endif
