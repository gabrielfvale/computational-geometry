#include "geometry.h"
#include <istream>

using namespace std;

Geometry::Geometry() {}

Geometry::Geometry(vector<vector<SDL_FPoint>> &hulls, double eps)
{
  if (hulls.size() == 0)
  {
    return;
  }

  vector<SDL_FPoint> all_points = {};
  for (auto hull : hulls)
    for (auto point : hull)
      all_points.push_back(point);

  if (all_points.size() < 3)
  {
    return;
  }

  points.push_back(all_points[0]);
  points.push_back(all_points[all_points.size() - 1]);
  int min_y = points[0].y;
  if ((points[1].y < min_y) || (min_y == points[1].y) && points[1].x < points[0].x)
  {
    min_y = points[1].y;
    swap(points[0], points[1]);
  }
  SDL_FPoint p0 = points[0];

  for (int i = 1; i < all_points.size() - 1; ++i)
  {
    SDL_FPoint p1 = all_points[i];
    bool exists = false;
    for (SDL_FPoint p2 : points)
    {
      double dist = squared_dist(p1, p2);
      if (squared_dist(p1, p2) < eps)
      {
        exists = true;
        break;
      }
    }
    if (!exists)
    {
      // Insert new point
      points.push_back(p1);
      // Find if newly inserted point is new min_point
      int last = points.size() - 1;
      int y = points[last].y;
      if ((y < min_y) || (min_y == y) && points[last].x < points[0].x)
      {

        min_y = points[last].y;
        swap(points[0], points[last]);
      }
      // Set p0 to min_point
      p0.x = points[0].x;
      p0.y = points[0].y;

      // Sort by counter-clockwise
      if (comp(p0, points[last - 1], points[last]))
      {
        swap(points[last - 1], points[last]);
      }
    }
  }
  // Add edges
  for (int i = 0; i < points.size() - 1; ++i)
  {
    edges.push_back(make_pair(i, i + 1));
  }
  edges.push_back(make_pair(points.size() - 1, 0));
}

void Geometry::out_point(SDL_FPoint &p)
{
  cout << "(" << p.x << ", " << p.y << ")" << endl;
}

double Geometry::squared_dist(SDL_FPoint &p1, SDL_FPoint &p2)
{
  return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

int Geometry::orientation(SDL_FPoint &a, SDL_FPoint &b, SDL_FPoint &c)
{
  int value = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
  // Colinear
  if (value == 0)
    return 0;
  // CW or CCW, respectively
  return (value > 0) ? 1 : 2;
};

bool Geometry::comp(SDL_FPoint &p0, SDL_FPoint &p1, SDL_FPoint &p2)
{
  int ori = orientation(p0, p1, p2);
  if (ori == 0)
  {
    cout << "they colinear" << endl;
    return (squared_dist(p0, p2) >= squared_dist(p0, p1)) ? false : true;
  }
  return (ori == 2) ? false : true;
}

void Geometry::renderPoints(GLfloat size)
{
  glPointSize(size);
  glBegin(GL_POINTS);
  glColor3f(1, 1, 1);
  for (auto point : points)
  {
    glVertex2f(point.x, point.y);
  }
  glEnd();
}
void Geometry::renderEdges()
{
  glColor3f(1, 1, 1);
  glBegin(GL_LINES);
  for (auto edge : edges)
  {
    auto p1 = points[edge.first];
    auto p2 = points[edge.second];
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
  }
  glEnd();
}

void Geometry::renderPolygon()
{
  glColor3f(1, 1, 1);
  glBegin(GL_POLYGON);
  for (auto point : points)
  {
    glVertex2f(point.x, point.y);
  }
  glEnd();
}
