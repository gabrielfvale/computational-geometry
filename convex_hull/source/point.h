#ifndef POINT_H
#define POINT_H

#include <iostream>

struct Point
{
  double x;
  double y;
  Point() {}
  Point(double x, double y)
  {
    this->x = x;
    this->y = y;
  }
  friend std::ostream &operator<<(std::ostream &os, const Point &p)
  {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
  }
};

#endif
