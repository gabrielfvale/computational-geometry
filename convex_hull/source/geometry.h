#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "point.h"

using namespace std;

struct Comparator
{
  Point p0;

  Comparator(Point ref) : p0(ref) {}

  static double squared_dist(Point &p1, Point &p2)
  {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
  }

  int orientation(Point &a, Point &b, Point &c)
  {
    int value = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    // Colinear
    if (value == 0)
      return 0;
    // CW or CCW, respectively
    return (value > 0) ? 1 : 2;
  };

  bool comp(Point &p0, Point &p1, Point &p2)
  {
    int ori = orientation(p0, p1, p2);
    if (ori == 0)
    {
      return (squared_dist(p0, p2) >= squared_dist(p0, p1)) ? false : true;
    }
    return (ori == 2) ? false : true;
  }

  bool operator()(Point &p1, Point &p2)
  {
    return comp(p0, p1, p2);
  }
};

struct Edge
{
  pair<int, int> vertices;
  bool visited;
  Edge(int a, int b)
  {
    this->vertices = make_pair(a, b);
    this->visited = false;
  };
  bool pLiesIn(int i, vector<Point> &points, double eps = 0.001)
  {
    auto p = points[i];
    auto v1 = points[vertices.first];
    auto v2 = points[vertices.second];

    if (!((v1.x <= p.x && p.x <= v2.x) || (v2.x <= p.x && p.x <= v1.x)))
    {
      return false;
    }
    if (!((v1.y <= p.y && p.y <= v2.y) || (v2.y <= p.y && p.y <= v1.y)))
    {
      return false;
    }

    double dot = (v1.x - p.x) * (v2.y - p.y) - (v1.y - p.y) * (v2.x - p.x);

    int dx1 = v2.x - v1.x;
    int dy1 = v2.y - v1.y;
    double epsilon = 0.003 * (dx1 * dx1 + dy1 * dy1);

    return abs(dot) < eps;
  };
  bool sameEdge(Edge &e2, vector<Point> &points)
  {
    if (vertices.first == e2.vertices.first && vertices.second == e2.vertices.second)
    {
      return true;
    }
    if (vertices.second == e2.vertices.first && vertices.first == e2.vertices.second)
    {
      return true;
    }

    auto e1p1 = points[vertices.first];
    auto e1p2 = points[vertices.second];
    auto e2p1 = points[e2.vertices.first];
    auto e2p2 = points[e2.vertices.second];
    double eps = 0.01;

    if (e1p1.squared_dist(e2p1) <= eps && e1p2.squared_dist(e2p2) <= eps)
    {
      return true;
    }

    if (e1p1.squared_dist(e2p2) <= eps && e1p2.squared_dist(e2p1) <= eps)
    {
      return true;
    }

    return false;
  }
  friend std::ostream &operator<<(std::ostream &os, const Edge &e)
  {
    os << "P: " << e.vertices.first << ", " << e.vertices.second << " V: " << e.visited;
    return os;
  };
};

class Geometry
{
private:
  static Point p0;

public:
  vector<Point> points = {};
  vector<pair<int, int>> edges = {};
  vector<Edge> test_edges = {};
  Geometry();
  Geometry(vector<vector<Point>> &hulls, double eps = 1);
  void renderPoints(GLfloat size = 2);
  void renderEdges();
  void renderPolygon();
  void render();
};

#endif
