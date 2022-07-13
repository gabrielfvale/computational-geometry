#ifndef COMPARATOR_H
#define COMPARATOR_H

struct Comparator
{
  Point p0;

  Comparator(Point ref) : p0(ref) {}

  static double squared_dist(Point &p1, Point &p2)
  {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
  }

  int orientation(Point &a, Point &b, Point &c)
  {
    int value = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    // Colinear
    if (value == 0)
      return 0;
    // CW or CCW, respectively
    return (value > 0) ? 1 : 2;
  };

  bool comp(Point &p0, Point &p1, Point &p2)
  {
    int ori = orientation(p0, p1, p2);
    if (ori == 0)
    {
      return (squared_dist(p0, p2) >= squared_dist(p0, p1)) ? true : false;
    }
    return (ori == 2) ? true : false;
  }

  bool operator()(Point &p1, Point &p2)
  {
    return comp(p0, p1, p2);
  }
};

#endif
