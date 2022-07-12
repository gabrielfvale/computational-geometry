#include "geometry.h"
#include <algorithm>
#include <istream>

#include "hullbuffer.h"

using namespace std;

Geometry::Geometry() {}

/*
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
      test_edges[i].visited = true;
      for (int j = 1; j < test_edges.size(); ++j)
      {
        if (i != j)
        {
          pair<int, int> ev = test_edges[i].vertices;
          pair<int, int> e2v = test_edges[j].vertices;

          // First case: edge contains another
          bool intersect1 = test_edges[i].pLiesInByDist(e2v.first, points);
          bool intersect2 = test_edges[i].pLiesInByDist(e2v.second, points);

          if (intersect1 && intersect2)
          {
            // cout << "Found and edge that has intersections" << endl;
            // cout << test_edges[i] << endl;
            // cout << test_edges[j] << endl;

            double eps = 0.01;
            // Repeated edges case
            if (test_edges[i].sameEdge(test_edges[j], points))
            {
              // cout << "same edge detected" << endl;
              test_edges.erase(test_edges.begin() + j);
              break;
            }
            test_edges.erase(test_edges.begin() + i);
            test_edges.erase(test_edges.begin() + j);
            double d1 = points[e2v.first].squared_dist(points[ev.first]);
            double d2 = points[e2v.second].squared_dist(points[ev.first]);
            if (d1 < d2)
            {
              test_edges.push_back(Edge(ev.first, e2v.first));
              test_edges.push_back(Edge(e2v.second, ev.second));
            }
            else
            {
              test_edges.push_back(Edge(ev.first, e2v.second));
              test_edges.push_back(Edge(ev.second, e2v.first));
            }
          }
        }
      }
    }
  }
}
*/
Geometry::Geometry(const vector<vector<Point>> &p)
{
  for (auto v : p)
  {
    parts.push_back(v);
  }
}

void Geometry::calc_hulls()
{
  for (size_t i = 0; i < parts.size(); ++i)
  {
    vector<int> buffer = HullBuffer::index_buffer(parts[i]);
    hulls.push_back(buffer);
  }
}

void Geometry::renderHulls()
{
  glBegin(GL_LINES);
  for (size_t i = 0; i < hulls.size(); ++i)
  {
    auto buffer = hulls[i];
    int buffer_size = buffer.size();
    auto part = parts[i];
    for (size_t j = 0; j < buffer_size - 1; ++j)
    {
      glVertex2f(part[buffer[j]].x, part[buffer[j]].y);
      glVertex2f(part[buffer[j + 1]].x, part[buffer[j + 1]].y);
    }
    glVertex2f(part[buffer[buffer_size - 1]].x, part[buffer[buffer_size - 1]].y);
    glVertex2f(part[buffer[0]].x, part[buffer[0]].y);
  }
  glEnd();
}

void Geometry::renderPoints(GLfloat size)
{
  glPointSize(size);
  glBegin(GL_POINTS);
  for (auto part : parts)
  {
    for (auto point : part)
    {
      glVertex2f(point.x, point.y);
    }
  }
  glEnd();
}

void Geometry::renderEdges()
{
  glBegin(GL_LINES);
  for (auto edge : test_edges)
  {
    auto p1 = points[edge.vertices.first];
    auto p2 = points[edge.vertices.second];
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
  }

  glEnd();
}

void Geometry::renderPolygon()
{
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
