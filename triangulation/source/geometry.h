#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "point.h"
#include "comparator.h"
#include "primitives.h"

using namespace std;

class Geometry
{
private:
public:
  vector<vector<Point>> parts;
  vector<vector<int>> hulls;
  vector<vector<Triangle>> triangles;

  Geometry();
  Geometry(const vector<vector<Point>> &p);
  ~Geometry();
  // Geometry(vector<vector<Point>> &hulls, double eps = 1);
  void calc_hulls();
  void triangulate();
  void renderHulls();
  void renderPoints(GLfloat size = 2);
  void renderEdges();
  void renderDebugEdge(int p1, int p2);
  void renderDebug(int v1, int v2, int p);
  void renderTriangles();
  void renderPolygon();
  void render();
};

#endif
