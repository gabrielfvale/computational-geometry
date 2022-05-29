#include "vector.h"
#include <math.h>

Vec3::Vector::Vector()
{
  coords[0] = 0;
  coords[1] = 0;
  coords[2] = 0;
}

Vec3::Vector::Vector(double coordinates[3])
{
  coords[0] = coordinates[0];
  coords[1] = coordinates[1];
  coords[2] = coordinates[2];
}

Vec3::Vector::Vector(float x, float y, float z)
{
  coords[0] = x;
  coords[1] = y;
  coords[2] = z;
}

double &Vec3::Vector::operator()(int i)
{
  return coords[i];
}

double Vec3::norm(Vector &x)
{
  double sum = 0;
  for (int i = 0; i < 3; ++i)
  {
    sum += x(i) * x(i);
  }
  return sqrt(sum);
}

Vec3::Vector Vec3::sum(Vector &x, Vector &y)
{

  double res[3];
  for (int i = 0; i < 3; ++i)
  {
    res[i] = x(i) + y(i);
  }
  return Vec3::Vector(res);
}

Vec3::Vector Vec3::mult_scalar(double s, Vector &x)
{
  double res[3];
  for (int i = 0; i < 3; ++i)
  {
    res[i] = s * x(i);
  }
  return Vec3::Vector(res);
}

double Vec3::dot_prod(Vector &x, Vector &y)
{

  double sum = 0;
  for (int i = 0; i < 3; ++i)
  {
    sum += x(i) * y(i);
  }
  return sum;
}

Vec3::Vector Vec3::cross_prod(Vector &x, Vector &y)
{
  return Vector(
      x(1) * y(2) - x(2) * y(1),
      x(2) * y(0) - x(0) * y(2),
      x(0) * y(1) - x(1) * y(0));
}
