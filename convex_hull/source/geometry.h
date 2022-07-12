#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "point.h"
#include "comparator.h"

using namespace std;

struct Edge
{
  pair<int, int> vertices;
  bool visited;
  Edge(int a, int b)
  {
    this->vertices = make_pair(a, b);
    this->visited = false;
  };
  bool pLiesIn(int i, vector<Point> &points)
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

    return abs(dot) < 0.01;
  };
  double pDist(int i, vector<Point> &points)
  {
    auto p = points[i];
    auto v1 = points[vertices.first];
    auto v2 = points[vertices.second];

    double eps = 0.001;

    double A = p.x - v1.x;
    double B = p.y - v1.y;
    double C = v2.x - v1.x;
    double D = v2.y - v1.y;

    double dot = A * C + B * D;
    double len_sq = C * C + D * D;
    double param = -1;
    if (len_sq != 0)
      param = dot / len_sq;

    double xx, yy;

    if (param < 0)
    {
      xx = v1.x;
      yy = v1.y;
    }
    else if (param > 1)
    {
      xx = v2.x;
      yy = v2.y;
    }
    else
    {
      xx = v1.x + param * C;
      yy = v1.y + param * D;
    }

    double dx = p.x - xx;
    double dy = p.y - yy;

    return (dx * dx + dy * dy);
  };
  bool pLiesInByDist(int i, vector<Point> &points, double eps = 0.01)
  {
    return this->pDist(i, points) <= eps;
  }
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
public:
  vector<Point> points = {};
  vector<pair<int, int>> edges = {};
  vector<Edge> test_edges = {};

  vector<vector<Point>> parts;
  vector<vector<int>> hulls;
  Geometry();
  Geometry(const vector<vector<Point>> &p);
  // Geometry(vector<vector<Point>> &hulls, double eps = 1);
  void calc_hulls();
  void renderHulls();
  void renderPoints(GLfloat size = 2);
  void renderEdges();
  void renderPolygon();
  void render();
};

#endif
