#include "geometry.h"
#include <algorithm>
#include <istream>

using namespace std;

Geometry::Geometry() {}

Geometry::Geometry(vector<vector<Point>> &hulls, double eps)
{
  if (hulls.size() == 0)
  {
    return;
  }

  vector<Point> all_points = {};
  for (auto hull : hulls)
    for (auto point : hull)
      all_points.push_back(point);

  if (all_points.size() < 3)
  {
    return;
  }

  int prev_hull_size = 0;
  for (auto hull : hulls)
  {
    auto s = hull.size() - 1;
    for (int i = 0; i < s; ++i)
    {
      int offset = prev_hull_size + i;
      test_edges.push_back(Edge(offset, offset + 1));
    }
    test_edges.push_back(Edge(prev_hull_size + s, prev_hull_size));
    prev_hull_size += s + 1;
  }

  points = all_points;

  vector<Edge> t;
  for (int i = 0; i < test_edges.size(); ++i)
  {
    if (!test_edges[i].visited)
    {
      bool intersects = false;
      for (int j = 0; j < test_edges.size(); ++j)
      {
        if (j != i && !test_edges[j].visited)
        {
          pair<int, int> ev = test_edges[i].vertices;
          pair<int, int> e2v = test_edges[j].vertices;
          bool intersect1 = test_edges[i].pLiesIn(e2v.first, points);
          bool intersect2 = test_edges[i].pLiesIn(e2v.second, points);
          if (intersect1 && intersect2)
          {
            cout << "Found and edge that has intersections" << endl;
            cout << test_edges[i] << endl;
            cout << test_edges[j] << endl;

            double eps = 0.01;
            // Repeated edges case
            if (test_edges[i].sameEdge(test_edges[j], points))
            {
              // cout << "same edge detected" << endl;
              intersects = true;
              test_edges[i].visited = true;
              test_edges[j].visited = true;
              break;
            }
            double d1 = points[e2v.first].squared_dist(points[ev.first]);
            double d2 = points[e2v.second].squared_dist(points[ev.first]);
            if (d1 < d2)
            {
              t.push_back(Edge(ev.first, e2v.first));
              t.push_back(Edge(e2v.second, ev.second));
            }
            else
            {
              t.push_back(Edge(ev.first, e2v.second));
              t.push_back(Edge(ev.second, e2v.first));
            }
            intersects = true;
            test_edges[j].visited = true;
          }
        }
      }
      if (!intersects)
      {
        t.push_back(test_edges[i]);
      }
      test_edges[i].visited = true;
    }
  }

  test_edges = t;

  // points.push_back(all_points[0]);
  // points.push_back(all_points[all_points.size() - 1]);
  // int min_y = points[0].y;
  // if ((points[1].y < min_y) || (min_y == points[1].y) && points[1].x < points[0].x)
  // {
  //   min_y = points[1].y;
  //   swap(points[0], points[1]);
  // }
  // Point p0 = points[0];

  // for (unsigned int i = 1; i < all_points.size() - 1; ++i)
  // {
  //   Point p1 = all_points[i];
  //   bool exists = false;
  //   for (Point p2 : points)
  //   {
  //     double dist = Comparator::squared_dist(p1, p2);
  //     if (Comparator::squared_dist(p1, p2) < eps)
  //     {
  //       exists = true;
  //       break;
  //     }
  //   }
  //   if (!exists)
  //   {
  //     // Insert new point
  //     points.push_back(p1);
  //     // Find if newly inserted point is new min_point
  //     int last = points.size() - 1;
  //     int y = points[last].y;
  //     if ((y < min_y) || (min_y == y) && points[last].x < points[0].x)
  //     {
  //       min_y = points[last].y;
  //       swap(points[0], points[last]);
  //     }
  //   }
  // }
  // // Set p0 to min_point
  // // p0.x = points[0].x;
  // // p0.y = points[0].y;
  // sort(points.begin(), points.end(), Comparator(points[0]));
  // // Add edges
  // for (unsigned int i = 0; i < points.size() - 1; ++i)
  // {
  //   edges.push_back(make_pair(i, i + 1));
  // }
  // edges.push_back(make_pair(points.size() - 1, 0));
}

void Geometry::renderPoints(GLfloat size)
{
  glPointSize(size);
  glBegin(GL_POINTS);
  glColor3f(1, 0, 0);
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
  // for (auto edge : test_edges)
  // {
  //   auto p1 = points[edge.vertices.first];
  //   auto p2 = points[edge.vertices.second];
  //   glVertex2f(p1.x, p1.y);
  //   glVertex2f(p2.x, p2.y);
  // }
  auto p1 = points[1];
  auto p2 = points[2];
  glVertex2f(p1.x, p1.y);
  glVertex2f(p2.x, p2.y);
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

void Geometry::render()
{
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // renderPolygon();
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  renderEdges();
  renderPoints(4.0);
}
