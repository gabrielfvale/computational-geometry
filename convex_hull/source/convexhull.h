#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
#include <tuple>

#include "point.h"

Point p0;
Point secondTop(std::stack<Point> &s)
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

int squaredDist(Point p1, Point p2)
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
    return (squaredDist(p0, *p2) >= squaredDist(p0, *p1)) ? -1 : 1;
  return (ori == 2) ? -1 : 1;
}

static std::tuple<Point *, int> convex_hull(Point points[], int size)
{
  std::vector<Point> convex_hull_points;

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
  p0 = points[0];
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
    return std::make_tuple(nullptr, 0);

  // Otherwise continue
  std::stack<Point> s;
  s.push(points[0]);
  s.push(points[1]);
  s.push(points[2]);

  for (int i = 3; i < n; ++i)
  {
    while (s.size() > 1 && orientation(secondTop(s), s.top(), points[i]) != 2)
      s.pop();
    s.push(points[i]);
  }

  std::vector<Point> v;
  while (!s.empty())
  {
    Point p = s.top();
    v.push_back(p);
    s.pop();
  }

  Point *result = new Point[v.size()];
  memcpy(result, &v.front(), v.size() * sizeof(Point));

  return std::make_tuple(result, v.size());
}

static std::vector<Point> convex_hull2(std::vector<Point> input)
{
  std::vector<Point> convex_hull_points;

  Point *points = new Point[input.size()];
  memcpy(points, &input.front(), input.size() * sizeof(Point));
  int size = input.size();

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
  p0 = points[0];
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
    return {};

  // Otherwise continue
  std::stack<Point> s;
  s.push(points[0]);
  s.push(points[1]);
  s.push(points[2]);

  for (int i = 3; i < n; ++i)
  {
    while (s.size() > 1 && orientation(secondTop(s), s.top(), points[i]) != 2)
      s.pop();
    s.push(points[i]);
  }

  std::vector<Point> v;
  while (!s.empty())
  {
    Point p = s.top();
    v.push_back(p);
    s.pop();
  }

  return v;
}

std::vector<std::vector<Point>> joined_convex_hull(std::vector<std::vector<Point>> object)
{
  std::vector<std::vector<Point>> hulls = {};
  for (auto part : object)
  {
    auto hull = convex_hull2(part);
    hulls.push_back(hull);
  }
  return hulls;
}

#endif
