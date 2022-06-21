
#ifndef GRAHAM_H
#define GRAHAM_H

#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <string.h>
#include "Point.h"

Point p0;

Point second_top(std::stack<Point> &s)
{
  Point temp_point = s.top();
  s.pop();
  Point res = s.top();
  s.push(temp_point);
  return res;
}

void swap(Point &p1, Point &p2)
{
  Point temp = p1;
  p1 = p2;
  p2 = temp;
}

int squared_dist(Point p1, Point p2)
{
  return ((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

int orientation(Point a, Point b, Point c)
{
  int value = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
  // Colinear
  if (value == 0)
    return 0;
  // CW or CCW, respectively
  return (value > 0) ? 1 : 2;
}

int comp(const void *point1, const void *point2)
{
  Point *p1 = (Point *)point1;
  Point *p2 = (Point *)point2;
  int ori = orientation(p0, *p1, *p2);
  if (ori == 0)
    return (squared_dist(p0, *p2) >= squared_dist(p0, *p1)) ? -1 : 1;
  return (ori == 2) ? -1 : 1;
}

std::vector<Point> graham(std::vector<Point> &input, int size)
{

  std::vector<Point> hull;
  Point *points = new Point[input.size()];
  memcpy(points, &input.front(), input.size() * sizeof(Point));

  // Iterate over points to find bottom-most or left-most
  int min_y = points[0].y, min = 0;
  for (int i = 1; i < size; ++i)
  {
    int y = points[i].y;
    if ((y < min_y) || (min_y == y) && points[i].x < points[min].x)
    {
      min_y = points[i].y;
      min = i;
    }
  }

  // Swap min point
  swap(points[0], points[min]);

  // Sort
  p0.x = points[0].x;
  p0.y = points[0].y;
  std::qsort(&points[1], size - 1, sizeof(Point), comp);

  int n = 1;

  for (int i = 1; i < size; ++i)
  {
    while (i < n - 1 && orientation(p0, points[i], points[i + 1]) == 0)
      i++;
    points[n] = points[i];
    n++;
  }

  // Case when convex hull is not possible
  if (n < 3)
    return std::vector<Point>{};

  // Otherwise continue
  std::stack<Point> s;
  s.push(points[0]);
  s.push(points[1]);
  s.push(points[2]);

  for (int i = 3; i < n; ++i)
  {
    while (s.size() > 1 && orientation(second_top(s), s.top(), points[i]) != 2)
      s.pop();
    s.push(points[i]);
  }

  while (!s.empty())
  {
    Point p = s.top();
    hull.push_back(p);
    s.pop();
  }
  return hull;
}

#endif
