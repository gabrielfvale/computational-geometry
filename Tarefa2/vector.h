#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <vector>

namespace Vec3
{
  class Vector
  {
  private:
    double coords[3];

  public:
    Vector();
    Vector(double coordinates[3]);
    Vector(float x, float y, float z);
    double &operator()(int i);
  };

  double norm(Vector &x);
  Vector sum(Vector &x, Vector &y);
  Vector mult_scalar(double s, Vector &x);
  double dot_prod(Vector &x, Vector &y);
  Vector cross_prod(Vector &x, Vector &y);
}

#endif
