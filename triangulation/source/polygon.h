#ifndef POLYGON_H
#define POLYGON_H

#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glew.h>

#include "point.h"

using namespace std;

class Polygon
{
private:
  vector<Point> points;

  int isVertexEar(int n, const vector<Point> &p)
  {
    return (isVertexInsideNewPoly(n, p) && !isEdgeIntersect(n, p));
  }

  int isEdgeIntersect(int n, const vector<Point> &p)
  {
    Point v = p[n];
    vector<Point> a;

    for (size_t i = 0; i < p.size(); ++i)
    {
      if (i != n)
      {
        a.push_back(p[i]);
      }
    }

    int c = 0, cnt = a.size(), prev = (cnt + (n - 1)) % cnt, next = n % cnt;

    Point v1 = a[prev], v2 = a[next];

    for (size_t i = 0, j = cnt - 1; i < cnt; j = i++)
    {
      if (prev == i || prev == j || next == i || next == j)
        continue;
      Point v4 = a[j], v3 = a[i];

      float d = ((v4.y - v3.y) * (v2.x - v1.x)) - ((v4.x - v3.x) * (v2.y - v1.y));
      if (!d)
        continue;

      float ua = (((v4.x - v3.x) * (v1.y - v3.y)) - ((v4.y - v3.y) * (v1.x - v3.x))) / d;
      float ub = (((v2.x - v1.x) * (v1.y - v3.y)) - ((v2.y - v1.y) * (v1.x - v3.x))) / d;

      if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1)
      {
        c = 1;
        break;
      }
    }
    return c;
  }

  int isVertexInsideNewPoly(int n, const vector<Point> &p)
  {
    Point v = p[n];
    vector<Point> a;

    for (size_t i = 0; i < p.size(); ++i)
    {
      if (i != n)
      {
        a.push_back(p[i]);
      }
    }

    int c = 1;

    for (size_t i = 0, j = a.size() - 1; i < a.size(); j = i++)
    {
      if (
          (((a[i].y <= v.y) && (v.y < a[j].y)) ||
           ((a[j].y <= v.y) && (v.y < a[i].y))) &&
          (v.x > (a[j].x - a[i].x) * (v.y - a[i].y) / (a[j].y - a[i].y) + a[i].x))
      {
        c = !c;
      }
    }

    return c;
  }

  float dist(Point a, Point b)
  {
    return sqrt(((a.x - b.x) * (a.x - b.x)) + (((a.y - b.y) * (a.y - b.y))));
  }

public:
  Polygon(const vector<Point> &p)
  {
    for (auto point : p)
    {
      points.push_back(point);
    }
  }

  Polygon(const vector<vector<Point>> &hulls)
  {
    for (auto hull : hulls)
      for (auto point : hull)
        points.push_back(point);
  }

  void renderPoints(GLfloat size)
  {
    glPointSize(size);
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    for (auto point : points)
    {
      glVertex2f(point.x, point.y);
    }
    glEnd();
  }

  void renderEdges()
  {
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    for (size_t i = 0; i < points.size() - 1; ++i)
    {
      glVertex2f(points[i].x, points[i].y);
      glVertex2f(points[i + 1].x, points[i + 1].y);
    }
    glVertex2f(points[points.size() - 1].x, points[points.size() - 1].y);
    glVertex2f(points[0].x, points[0].y);

    glEnd();
  }

  void renderTriangles()
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBegin(GL_TRIANGLES);
    glColor3f(0, 1, 0);
    for (auto point : points)
    {
      glVertex2f(point.x, point.y);
    }

    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  void push(const Point &p)
  {
    for (size_t i = 0; i < points.size(); i++)
    {
      if (dist(points[i], p) < 7.f)
      {
        points.push_back(p);
        return;
      }
    }
    points.push_back(p);
  }

  void pop()
  {
    if (points.size() > 0)
      points.pop_back();
  }

  void clear()
  {
    points.clear();
  }

  Point v(int index)
  {
    return points[index];
  }

  size_t size()
  {
    return points.size();
  }

  void triangulate()
  {
    cout << "Starting triangulation" << endl;
    vector<Point> newPoly;

    for (size_t i = 0; i < points.size(); ++i)
    {
      newPoly.push_back(points[i]);
    }

    points.clear();

    for (size_t t = newPoly.size() - 1, i = 0, j = 1; i < newPoly.size(); t = i++, j = (i + 1) % newPoly.size())
    {
      cout << t << ", " << i << ", " << j << endl;
      if (newPoly.size() == 3)
      {
        cout << "Min triangulation reached" << endl;
        points.push_back(newPoly[0]);
        points.push_back(newPoly[1]);
        points.push_back(newPoly[2]);
        break;
      }

      if (isVertexEar(i, newPoly))
      {
        cout << "isVertexEar! " << i << endl;
        points.push_back(newPoly[t]);
        points.push_back(newPoly[i]);
        points.push_back(newPoly[j]);

        // Update front
        newPoly.erase(newPoly.begin() + i, newPoly.begin() + i + 1);

        // Reset variables to move to new front
        t = newPoly.size() - 1;
        i = 0;
        j = 1;
      }
    }
  }
};

#endif
