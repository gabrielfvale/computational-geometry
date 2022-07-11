#ifndef POINT_H
#define POINT_H

#include <iostream>

struct Point
{
  double x;
  double y;
  Point(){};
  Point(double x, double y)
  {
    this->x = x;
    this->y = y;
  };
  double squared_dist(Point &p2)
  {
    return (x - p2.x) * (x - p2.x) + (y - p2.y) * (y - p2.y);
  }
  friend std::ostream &operator<<(std::ostream &os, const Point &p)
  {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
  };
};

#endif
