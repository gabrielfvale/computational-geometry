#ifndef QUICKHULL_H
#define QUICKHULL_H

#include <bits/stdc++.h>
#include "Point.h"

std::set<Point> hull;

int findSide(Point p1, Point p2, Point p)
{
  int val = (p.y - p1.y) * (p2.x - p1.x) -
            (p2.y - p1.y) * (p.x - p1.x);

  if (val > 0.0)
    return 1;
  if (val < 0.0)
    return -1;
  return 0;
}

int lineDist(Point p1, Point p2, Point p)
{
  return abs((p.y - p1.y) * (p2.x - p1.x) -
             (p2.y - p1.y) * (p.x - p1.x));
}

void quickHullRoutine(std::vector<Point> &a, int n, Point p1, Point p2, int side)
{
  int index = -1;
  int max_dist = 0;

  for (int i = 0; i < n; ++i)
  {
    int dist = lineDist(p1, p2, a[i]);
    if (findSide(p1, p2, a[i]) == side && dist > max_dist)
    {
      index = i;
      max_dist = dist;
    }
  }

  if (index == -1)
  {
    hull.insert(p1);
    hull.insert(p2);
    return;
  }

  quickHullRoutine(a, n, a[index], p1, -findSide(a[index], p1, p2));
  quickHullRoutine(a, n, a[index], p2, -findSide(a[index], p2, p1));
}

std::vector<Point> quickHull(std::vector<Point> &a, int n)
{
  if (n < 3)
    return std::vector<Point>{};

  std::vector<Point> result_hull;

  int min_x = 0,
      max_x = 0;
  for (int i = 1; i < n; ++i)
  {
    if (a[i].x < a[min_x].x)
    {
      min_x = i;
    }
    if (a[i].x > a[max_x].x)
    {
      max_x = i;
    }
  }

  quickHullRoutine(a, n, a[min_x], a[max_x], 1);

  quickHullRoutine(a, n, a[min_x], a[max_x], -1);

  for (auto p : hull)
  {
    result_hull.push_back(p);
  }

  return result_hull;
}

#endif
