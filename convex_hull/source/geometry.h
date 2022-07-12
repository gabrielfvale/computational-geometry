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
  Edge(){};
  Edge(int a, int b)
  {
    this->vertices = make_pair(a, b);
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
  int nearest(vector<Point> &points)
  {
    if (points.size() == 0)
    {
      return -1;
    }

    int min = 0;
    double dist = pDist(min, points);
    for (size_t i = 1; i < points.size(); ++i)
    {
      if (i != vertices.first && i != vertices.second)
      {
        double d = pDist(i, points);
        if (d < dist)
        {
          dist = d;
          min = i;
        }
      }
    }

    return min;
  }
  bool onSegment(Point p, Point q, Point r)
  {
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
      return true;

    return false;
  }
  static int orientation(Point &a, Point &b, Point &c)
  {
    int value = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    // Colinear
    if (value == 0)
      return 0;
    // CW or CCW, respectively
    return (value > 0) ? 1 : 2;
  };
  bool intersects(const Edge &e, const vector<Point> &points)
  {
    // cout << endl;
    // cout << "P1Q1: " << vertices.first << ", " << vertices.second << endl;
    // cout << "P2Q2: " << e.vertices.first << ", " << e.vertices.second << endl;
    if ((vertices.first == e.vertices.first && vertices.second == e.vertices.second) ||
        (vertices.first == e.vertices.second && vertices.second == e.vertices.first))
    {
      return false;
    }

    Point p1 = points[vertices.first];
    Point q1 = points[vertices.second];

    Point p2 = points[e.vertices.first];
    Point q2 = points[e.vertices.second];

    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // cout << "Orientations: " << o1 << ", " << o2 << ", " << endl;
    // cout << endl;
    // if ((o1 == o3 && o2 != o4) || (o2 == o4 && o1 != o3))
    // {
    //   return false;
    // }
    if (o1 != o2 && o3 != o4)
    {
      return true;
    }

    // Special Cases
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1))
      return true;

    // p1, q1 and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1))
      return true;

    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2))
      return true;

    // p2, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2))
      return true;
    return false;
  }
  bool intersects(Point p1, Point q1, Point p2, Point q2)
  {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
    {
      return true;
    }

    // Special Cases
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1))
      return true;

    // p1, q1 and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1))
      return true;

    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2))
      return true;

    // p2, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2))
      return true;
    return false;
  }
  friend std::ostream &operator<<(std::ostream &os, const Edge &e)
  {
    os << "P: " << e.vertices.first << ", " << e.vertices.second;
    return os;
  }
};

struct Triangle
{
  int v1, v2, v3;
  Triangle(int p0, int p1, int p2)
  {
    v1 = p0;
    v2 = p1;
    v3 = p2;
  }
  double sign(Point p1, Point p2, Point p3)
  {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
  }
  double side(double x1, double y1, double x2, double y2, double x, double y)
  {
    return (y2 - y1) * (x - x1) + (-x2 + x1) * (y - y1);
  }
  bool intersects(Triangle &t, const vector<Point> &points)
  {
    // vector<int> i1 = {v1, v2, v3};
    // vector<int> i2 = {t.v1, t.v2, t.v3};
    // Point n1 = Point(points[v2].x - points[v1].x, points[v2].y - points[v1].y);
    vector<Edge> e1 = {Edge(v1, v2), Edge(v2, v3), Edge(v3, v1)};
    vector<Edge> t1 = {Edge(t.v1, t.v2), Edge(t.v2, t.v3), Edge(t.v3, t.v1)};

    Point p0 = points[v1];
    Point p1 = points[v2];
    Point p2 = points[v3];

    // cout << "Prev points: " << endl;
    // cout << p0 << endl;
    // cout << p1 << endl;
    // cout << p2 << endl;

    // Offset points "inward"
    Point c = Point(
        (p0.x + p1.x + p2.x) / 3,
        (p0.y + p1.y + p2.y) / 3);

    Point n0 = Point(c.x - p0.x, c.y - p0.y);
    Point n1 = Point(c.x - p1.x, c.y - p1.y);
    Point n2 = Point(c.x - p2.x, c.y - p2.y);
    double n0l = sqrt(n0.x * n0.x + n0.y + n0.y);
    double n1l = sqrt(n1.x * n1.x + n1.y + n1.y);
    double n2l = sqrt(n2.x * n2.x + n2.y + n2.y);
    n0.x /= n0l > 0 ? n0l : 1;
    n0.y /= n0l > 0 ? n0l : 1;
    n1.x /= n1l > 0 ? n1l : 1;
    n1.y /= n1l > 0 ? n1l : 1;
    n2.x /= n2l > 0 ? n2l : 1;
    n2.y /= n2l > 0 ? n2l : 1;

    p0.x += n0.x;
    p0.y += n0.y;

    p1.x += n1.x;
    p1.y += n1.y;

    p2.x += n2.x;
    p2.y += n2.y;

    // cout << "Offset points: " << endl;
    // cout << p0 << endl;
    // cout << p1 << endl;
    // cout << p2 << endl;

    Edge h;
    vector<Point> edges1 = {p0, p1, p2};
    vector<Point> edges2 = {points[t.v1], points[t.v2], points[t.v3]};

    for (int i = 0; i < 3; ++i)
    {
      int next_i = (i + 1) % 3;
      for (int j = 0; j < 3; ++j)
      {
        int next_j = (j + 1) % 3;
        if (h.intersects(edges1[i], edges1[next_i], edges2[j], edges2[next_j]))
        {
          // cout << "heres the edges that intercepted: " << i << ", " << j << endl;
          return true;
        }
      }
    }

    int in_triangle_sum = 0;
    for (int i = 0; i < 3; ++i)
    {
      Point p = edges1[i];
      if (t.pointInTriangle(p, points))
      {
        in_triangle_sum++;
      }
    }
    if (in_triangle_sum == 3)
      return true;

    // for (int i = 0; i < 3; ++i)
    // {
    //   for (int j = 0; j < 3; ++j)
    //   {
    //     if (e1[i].intersects(t1[j], points))
    //       return true;
    //   }
    // }

    // for (int i = 0; i < 3; ++i)
    // {
    //   for (int j = 0; j < 3; ++j)
    //   {
    //     if (e1[i].intersects(t1[j], points))
    //       return true;
    //   }
    // }
    // vector<int> t1 = {v1, v2, v3};
    // vector<int> t2 = {t.v1, t.v2, t.v3};
    // int equal_count = 0;
    // for (size_t i = 0; i < t1.size(); ++i)
    // {
    //   for (size_t j = 0; j < t1.size(); ++j)
    //   {
    //     if (t1[i] == t2[j])
    //       equal_count++;
    //   }
    // }
    // if (equal_count >= 3)
    // {
    //   return true;
    // }

    // for (int p : t1)
    // {
    //   Point k = points[p];
    //   if (pointInTriangle(k, points))
    //   {
    //     return true;
    //   }
    // }

    // for (int p : t2)
    // {
    //   Point k = points[p];
    //   if (t.pointInTriangle(k, points))
    //   {
    //     return true;
    //   }
    // }

    return false;
  }
  bool intersectsList(const vector<Triangle> &ts, const vector<Point> &points)
  {
    for (auto t : ts)
    {
      if (intersects(t, points))
        return true;
    }
    return false;
  }
  bool pointInTriangle(Point &p, const vector<Point> &points)
  {
    // for (size_t i = 0; i < points.size(); ++i)
    // {
    //   if (i != v1 && i != v2 && i != v3)
    //   {
    //     Point p = points[i];
    //     float d1, d2, d3;
    //     bool has_neg, has_pos;

    //     d1 = sign(p, points[v1], points[v2]);
    //     d2 = sign(p, points[v2], points[v3]);
    //     d3 = sign(p, points[v3], points[v1]);

    //     has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    //     has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    //     if (!(has_neg && has_pos))
    //       return true;
    //   }
    // }

    Point p1 = points[v1];
    Point p2 = points[v2];
    Point p3 = points[v3];

    bool checkSide1 = side(p1.x, p1.y, p2.x, p2.y, p.x, p.y) >= 0;
    bool checkSide2 = side(p2.x, p2.y, p3.x, p3.y, p.x, p.y) >= 0;
    bool checkSide3 = side(p3.x, p3.y, p1.x, p1.y, p.x, p.y) >= 0;
    return checkSide1 && checkSide2 && checkSide3;
  }
  bool pointsInTriangle(const vector<Point> &points)
  {
    for (size_t i = 0; i < points.size(); ++i)
    {
      if (i != v1 && i != v2 && i != v3)
      {
        Point p = points[i];
        float d1, d2, d3;
        bool has_neg, has_pos;

        d1 = sign(p, points[v1], points[v2]);
        d2 = sign(p, points[v2], points[v3]);
        d3 = sign(p, points[v3], points[v1]);

        has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

        if (!(has_neg && has_pos))
          return true;
      }
    }
    return false;
  }
  friend std::ostream &operator<<(std::ostream &os, const Triangle &t)
  {
    os << "(" << t.v1 << ", " << t.v2 << ", " << t.v3 << ")";
    return os;
  }
};

struct Circle
{
  Point center;
  double radius;
  bool valid;

  Circle(Point p0, Point p1, Point p2)
  {
    double x12 = p0.x + 0.1 - p1.x;
    double x13 = p0.x + 0.1 - p2.x;

    double y12 = p0.y - p1.y;
    double y13 = p0.y - p2.y;

    double y31 = p2.y - p0.y;
    double y21 = p1.y - p0.y;

    double x31 = p2.x - p0.x + 0.1;
    double x21 = p1.x - p0.x + 0.1;

    // p0.x + 0.1^2 - p2.x^2
    double sx13 = pow(p0.x + 0.1, 2) - pow(p2.x, 2);

    // p0.y^2 - p2.y^2
    double sy13 = pow(p0.y, 2) - pow(p2.y, 2);

    double sx21 = pow(p1.x, 2) - pow(p0.x + 0.1, 2);
    double sy21 = pow(p1.y, 2) - pow(p0.y, 2);

    double d1 = (2 * ((y31) * (x12) - (y21) * (x13)));
    double d2 = (2 * ((x31) * (y12) - (x21) * (y13)));

    // cout << d1 << endl;

    if (d1 == 0 || d2 == 0)
    {
      // cout << "hey its invalid" << endl;
      valid = false;
      return;
    }

    double f = ((sx13) * (x12) + (sy13) * (x12) + (sx21) * (x13) + (sy21) * (x13)) / (2 * ((y31) * (x12) - (y21) * (x13)));
    double g = ((sx13) * (y12) + (sy13) * (y12) + (sx21) * (y13) + (sy21) * (y13)) / (2 * ((x31) * (y12) - (x21) * (y13)));

    double c = -pow(p0.x + 0.1, 2) - pow(p0.y, 2) - 2 * g * p0.x + 0.1 - 2 * f * p0.y;

    // eqn of circle be x^2 + y^2 + 2*g*x + 2*f*y + c = 0
    // where centre is (h = -g, k = -f) and radius r
    // as r^2 = h^2 + k^2 - c
    double h = -g;
    double k = -f;
    double sqr_of_r = h * h + k * k - c;

    // r is the radius
    float r = sqrt(sqr_of_r);

    center = Point(h, k);
    radius = r;
    valid = true;
  }
  bool inCircle(Point p)
  {
    double dx = abs(p.x - center.x);
    // cout << "dx : " << dx << ", " << radius << endl;
    if (dx > radius)
      return false;
    double dy = abs(p.y - center.y);
    if (dy > radius)
      return false;
    if (dx + dy <= radius)
      return true;
    return (dx * dx + dy * dy <= radius * radius);
  }
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
  vector<Triangle> triangles;

  Geometry();
  Geometry(const vector<vector<Point>> &p);
  // Geometry(vector<vector<Point>> &hulls, double eps = 1);
  void calc_hulls();
  void triangulate(int iter_count = 10);
  void renderHulls();
  void renderPoints(GLfloat size = 2);
  void renderEdges();
  void renderDebugEdge(int p1, int p2);
  void renderTriangles();
  void renderPolygon();
  void render();
};

#endif
