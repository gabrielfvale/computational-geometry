#ifndef JARVIS_H
#define JARVIS_H

#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
#include "Point.h"

int jarvis_orientation(Point a, Point b, Point c)
{
  int value = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
  // Colinear
  if (value == 0)
    return 0;
  // CW or CCW, respectively
  return (value > 0) ? 1 : 2;
}

std::vector<Point> jarvis(std::vector<Point> &points, int n)
{
  if (n < 3)
    return std::vector<Point>{};

  std::vector<Point> hull;

  // Iterate over points to find left-most
  int l = 0;
  for (int i = 1; i < n; i++)
  {
    if (points[i].x < points[l].x)
    {

      l = i;
    }
  }

  int p = l, q;
  do
  {
    hull.push_back(points[p]);

    q = (p + 1) % n;
    for (int i = 0; i < n; i++)
    {
      if (jarvis_orientation(points[p], points[i], points[q]) == 2)
        q = i;
    }

    p = q;

  } while (p != l);

  return hull;
}

#endif
