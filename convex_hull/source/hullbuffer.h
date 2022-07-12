#ifndef HULL_BUFFER_H
#define HULL_BUFFER_H

#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
#include <tuple>

#include "comparator.h"
#include "point.h"

namespace HullBuffer
{
  static Point secondTop(std::stack<Point> &s, std::stack<int> &s_buffer)
  {
    Point temp_point = s.top();
    int temp_pos = s_buffer.top();
    s.pop();
    s_buffer.pop();
    Point res = s.top();
    int buffer_res = s_buffer.top();
    s.push(temp_point);
    s_buffer.push(temp_pos);
    return res;
  }
  static int squaredDist(Point p1, Point p2)
  {
    return ((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
  }
  static int orientation(Point a, Point b, Point c)
  {
    int value = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    // Colinear
    if (value == 0)
      return 0;
    // CW or CCW, respectively
    return (value > 0) ? 1 : 2;
  }
  static std::vector<int> index_buffer(std::vector<Point> &points)
  {
    Point p0;

    // Iterate over points to find bottom-most or left-most
    int min_y = points[0].y, min = 0;
    for (int i = 1; i < points.size(); ++i)
    {
      int y = points[i].y;
      if ((y < min_y) || (min_y == y) && points[i].x < points[min].x)
      {
        min_y = points[i].y;
        min = i;
      }
    }
    cout << "min_y " << min_y << endl;

    // Swap min point
    std::swap(points[0], points[min]);

    // Sort
    p0 = points[0];
    std::sort(points.begin(), points.end(), Comparator(points[0]));

    int n = 1;

    for (size_t i = 1; i < points.size(); ++i)
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
    std::stack<int> s_buffer;
    s.push(points[0]);
    s.push(points[1]);
    s.push(points[2]);
    s_buffer.push(0);
    s_buffer.push(1);
    s_buffer.push(2);

    for (int i = 3; i < n; ++i)
    {
      while (s.size() > 1 && orientation(secondTop(s, s_buffer), s.top(), points[i]) != 2)
      {
        s.pop();
        s_buffer.pop();
      }
      s.push(points[i]);
      s_buffer.push(i);
    }

    std::vector<int> v;
    while (!s_buffer.empty())
    {
      int i = s_buffer.top();
      v.push_back(i);
      s_buffer.pop();
    }

    return v;
  }
};

#endif
