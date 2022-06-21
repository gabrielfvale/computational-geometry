#ifndef POINT_H
#define POINT_H

struct Point
{
  double x, y;
  bool operator<(const Point &other) const { return x < other.x; }
};

#endif
