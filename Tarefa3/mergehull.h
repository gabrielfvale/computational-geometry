#ifndef MERGEHULL_H
#define MERGEHULL_H

#include <bits/stdc++.h>
#include "Point.h"

Point mid;

int quad(Point p)
{
  if (p.x >= 0 && p.y >= 0)
    return 1;
  if (p.x <= 0 && p.y >= 0)
    return 2;
  if (p.x <= 0 && p.y <= 0)
    return 3;
  return 4;
}

int mhull_orientation(Point a, Point b,
                      Point c)
{
  int res = (b.y - a.y) * (c.x - b.x) -
            (c.y - b.y) * (b.x - a.x);

  if (res == 0)
    return 0;
  if (res > 0)
    return 1;
  return -1;
}

bool compare(Point p1, Point q1)
{
  Point p, q;
  p.x = p1.x - mid.x;
  p.y = p1.y - mid.y;

  q.x = q1.x - mid.x;
  q.y = q1.y - mid.y;

  int one = quad(p);
  int two = quad(q);

  if (one != two)
    return (one < two);
  return (p.y * q.x < q.y * p.x);
}

std::vector<Point> merger(std::vector<Point> a,
                          std::vector<Point> b)
{
  int n1 = a.size(), n2 = b.size();

  int ia = 0, ib = 0;
  for (int i = 1; i < n1; i++)
    if (a[i].x > a[ia].x)
      ia = i;

  for (int i = 1; i < n2; i++)
    if (b[i].x < b[ib].x)
      ib = i;

  // finding the upper tangent
  int inda = ia, indb = ib;
  bool done = 0;
  while (!done)
  {
    done = 1;
    while (mhull_orientation(b[indb], a[inda], a[(inda + 1) % n1]) >= 0)
      inda = (inda + 1) % n1;

    while (mhull_orientation(a[inda], b[indb], b[(n2 + indb - 1) % n2]) <= 0)
    {
      indb = (n2 + indb - 1) % n2;
      done = 0;
    }
  }

  int uppera = inda, upperb = indb;
  inda = ia, indb = ib;
  done = 0;
  int g = 0;
  while (!done)
  {
    done = 1;
    while (mhull_orientation(a[inda], b[indb], b[(indb + 1) % n2]) >= 0)
      indb = (indb + 1) % n2;

    while (mhull_orientation(b[indb], a[inda], a[(n1 + inda - 1) % n1]) <= 0)
    {
      inda = (n1 + inda - 1) % n1;
      done = 0;
    }
  }

  int lowera = inda, lowerb = indb;
  std::vector<Point> ret;

  int ind = uppera;
  ret.push_back(a[uppera]);
  while (ind != lowera)
  {
    ind = (ind + 1) % n1;
    ret.push_back(a[ind]);
  }

  ind = lowerb;
  ret.push_back(b[lowerb]);
  while (ind != upperb)
  {
    ind = (ind + 1) % n2;
    ret.push_back(b[ind]);
  }
  return ret;
}

std::vector<Point> bruteHull(std::vector<Point> a)
{

  std::set<Point> s;

  for (int i = 0; i < a.size(); i++)
  {
    for (int j = i + 1; j < a.size(); j++)
    {
      int x1 = a[i].x, x2 = a[j].x;
      int y1 = a[i].y, y2 = a[j].y;

      int a1 = y1 - y2;
      int b1 = x2 - x1;
      int c1 = x1 * y2 - y1 * x2;
      int pos = 0, neg = 0;
      for (int k = 0; k < a.size(); k++)
      {
        if (a1 * a[k].x + b1 * a[k].y + c1 <= 0)
          neg++;
        if (a1 * a[k].x + b1 * a[k].y + c1 >= 0)
          pos++;
      }
      if (pos == a.size() || neg == a.size())
      {
        s.insert(a[i]);
        s.insert(a[j]);
      }
    }
  }

  std::vector<Point> ret;
  for (auto e : s)
  {
    ret.push_back(e);
  }

  mid = {0, 0};
  int n = ret.size();
  for (int i = 0; i < n; i++)
  {
    mid.x += ret[i].x;
    mid.y += ret[i].y;
    ret[i].x *= n;
    ret[i].y *= n;
  }
  std::sort(ret.begin(), ret.end(), compare);
  for (int i = 0; i < n; i++)
  {
    ret[i].x = ret[i].x / n;
    ret[i].y = ret[i].y / n;
  }

  return ret;
}

std::vector<Point> mergeHull(std::vector<Point> &a)
{
  if (a.size() <= 5)
  {
    return bruteHull(a);
  }

  std::vector<Point> left, right;
  for (int i = 0; i < a.size() / 2; i++)
  {
    left.push_back(a[i]);
  }
  for (int i = a.size() / 2; i < a.size(); i++)
  {
    right.push_back(a[i]);
  }

  std::vector<Point> left_hull = mergeHull(left);
  std::vector<Point> right_hull = mergeHull(right);

  return merger(left_hull, right_hull);
}

#endif
