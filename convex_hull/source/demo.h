#ifndef DEMO_H
#define DEMO_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "point.h"
#include "comparator.h"
#include "geometry.h"

using namespace std;

class Demo
{
private:
  int steps = 0;
  vector<Point> points;
  vector<int> boundary;
  vector<Triangle> triangles;
  pair<int, int> selected_edge;

public:
  Demo();
  Demo(const vector<vector<Point>> &p);
  void triangulate(int step = 1);
  void render();
  void renderDebug(int p1, int p2, int p);
};

#endif
