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

void Geometry::triangulate(int step)
{
  auto hull = hulls[0];

  if (hulls[0].size() < 3)
  {
    return;
  }

  // if (hulls[0].size() == 3)
  // {
  //   int p1 = hull[0];
  //   int p2 = hull[1];
  //   int p3 = hull[2];
  //   Triangle t = Triangle(p1, p2, p3);
  //   triangles.push_back(t);
  //   hulls[0].clear();
  //   cout << "Min triangulation achieved" << endl;
  // }

  vector<int> boundary;
  for (auto i : hull)
    boundary.push_back(i);

  vector<Point> available;
  for (auto p : parts[0])
    available.push_back(p);

  int iteration = 0;
  int edge = 0;

  // Update boundary checking for colinear points
  for (size_t i = 0; i < available.size(); ++i)
  {
    for (size_t j = 0; j < boundary.size(); ++j)
    {
      int j_next = (j + 1) % boundary.size();
      if (i != boundary[j] && i != boundary[j_next])
      {

        Edge e = Edge(boundary[j], boundary[j_next]);
        double dist = e.pDist(i, available);
        if (e.pLiesInByDist(i, available, 1))
        {
          // cout << "There is a point that lies inside a boundary." << endl;
          boundary.insert(boundary.begin() + j_next, i);
          i = 0;
          break;
        }
      }
    }
  }

  while (boundary.size() > 3 && iteration < 2000 && (step == -1 || steps + 1 < step))
  {
    cout << endl;
    cout << "Iteration: " << iteration << endl;
    cout << "Boundary is: ";
    for (auto p : boundary)
      cout << p << ", ";
    cout << endl;

    cout << "Boundary is: ";
    for (auto p : boundary)
      cout << p << ", ";
    cout << endl;

    int edge_n = (edge + 1) % boundary.size();
    cout << "Edge " << edge << ": " << boundary[edge] << ", " << boundary[edge_n] << endl;
    ++iteration;
    if (edge >= boundary.size() - 1)
    {
      edge = 0;
    }
    int chosen_point = available.size() - 1;

    while (chosen_point == boundary[edge] || chosen_point == boundary[edge_n])
    {
      chosen_point = (chosen_point - 1) % available.size();
    }

    // Circle c = Circle(available[boundary[edge]], available[boundary[edge_n]], available[chosen_point]);
    // Triangle t = Triangle(boundary[edge], boundary[edge_n], chosen_point);
    // // c.inCircle(available[0]);
    // bool circle_empty = false;
    // while (chosen_point < available.size() - 1)
    // {
    //   if (chosen_point == boundary[edge] || chosen_point == boundary[edge_n])
    //   {
    //     chosen_point++;
    //     continue;
    //   }
    //   // cout << "creating circle for point " << chosen_point << endl;
    //   c = Circle(available[boundary[edge]], available[boundary[edge_n]], available[chosen_point]);
    //   // cout << "circle valid: " << c.valid << endl;
    //   int left_points = available.size();
    //   if (!c.valid)
    //   {
    //     chosen_point++;
    //     continue;
    //   }
    //   for (auto point : available)
    //   {
    //     left_points--;
    //     if (c.inCircle(point))
    //     {
    //       // cout << "a point is in this circle." << endl;
    //       // cout << endl;
    //       break;
    //     }
    //   }
    //   if (left_points == 0)
    //   {
    //     cout << "Circle is empty" << endl;
    //     t = Triangle(boundary[edge], boundary[edge_n], chosen_point);
    //     // cout << t << endl;
    //     if (!t.intersectsList(triangles, available))
    //     {
    //       circle_empty = true;
    //       cout << "Found a circle and triangle combo: " << t << endl;
    //       break;
    //     }
    //   }
    //   chosen_point++;
    // }

    auto t = Triangle(boundary[edge], boundary[edge_n], chosen_point);
    Edge helper;
    // cout << "P, I: " << t.pointsInTriangle(available) << ", " << t.intersectsList(triangles, available) << endl;
    while (
        chosen_point >= 0 &&
        (helper.orientation(boundary[edge], boundary[edge_n], chosen_point, available) == 0 ||
         (t.pointsInTriangle(available) || t.intersectsList(triangles, parts[0]))))
    {
      chosen_point--;
      // cout << boundary[edge] << ", " << boundary[edge_n] << ", " << chosen_point << endl;
      if (chosen_point == boundary[edge] || chosen_point == boundary[edge_n])
      {
        continue;
      }
      t = Triangle(boundary[edge], boundary[edge_n], chosen_point);
    }

    if (chosen_point < 0)
    {
      edge++;
      continue;
    }

    // cout << "Orientation: " << helper.orientation(boundary[edge], boundary[edge_n], chosen_point, available) << endl;

    // vector<int> created_edges = {boundary[edge], boundary[edge_n], chosen_point};
    // for (size_t i = 0; i < available.size(); ++i)
    // {
    //   for (size_t j = 0; j < created_edges.size(); ++j)
    //   {
    //     int next = (j + 1) % created_edges.size();

    //     if (i != created_edges[j] && i != created_edges[next])
    //     {
    //       Edge e = Edge(created_edges[j], created_edges[next]);
    //       double dist = e.pDist(i, available);
    //       if (e.pLiesInByDist(i, available, 1))
    //       {
    //         // cout << "There is a point that lies inside a boundary." << endl;
    //         boundary.insert(boundary.begin() + edge_n, i);
    //       }
    //     }
    //   }
    // }

    bool point_in_boundary = false;
    for (auto i : boundary)
    {
      if (i == chosen_point)
      {
        point_in_boundary = true;
        break;
      }
    }
    cout << "Point in boundary: " << point_in_boundary << endl;
    if (point_in_boundary)
    {
      int prev = HullBuffer::getIndex(boundary.size(), edge - 1);
      int next = HullBuffer::getIndex(boundary.size(), edge_n + 1);
      cout << "Prev, next: " << boundary[prev] << ", " << boundary[next] << endl;
      cout << "Chosen: " << chosen_point << endl;
      if (boundary[prev] == chosen_point)
      {
        cout << "Point is prev" << endl;
        boundary.erase(boundary.begin() + edge);
      }
      else if (boundary[next] == chosen_point)
      {
        cout << "Point is next" << endl;
        boundary.erase(boundary.begin() + edge_n);
      }
      else
      {
        // its a loop, continue.
        cout << "Extra case" << endl;
        Edge h;
        // Point e1 = points[boundary[edge]];
        // Point e2 = points[boundary[edge_n]];
        // Point c = points[chosen_point];
        if (helper.orientation(boundary[edge], boundary[edge_n], boundary[next], available) == 0)
        {
          cout << "INTERNAL LOOP" << endl;
          edge++;
          continue;
        }
        // h.orientation();
      }
    }
    else
    {
      boundary.insert(boundary.begin() + edge_n, chosen_point);
    }

    cout << "<-- INSERTING TRIANGLE --> " << endl;
    cout << t << endl;
    triangles.push_back(t);
    edge = 0;
    steps++;
  }

  if (boundary.size() == 3)
  {
    Triangle t = Triangle(boundary[0], boundary[1], boundary[2]);
    if (!t.pointsInTriangle(available))
    {
      triangles.push_back(t);
    }
  }

  hulls[0] = boundary;
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
    // glColor3f(0, 1, 0);
    // glVertex2f(part[buffer[0]].x, part[buffer[0]].y);
    // glVertex2f(part[buffer[1]].x, part[buffer[1]].y);
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

void Geometry::renderDebugEdge(int p1, int p2)
{
  // edge is red
  // first is blue, second is green,

  glColor3f(1, 0, 0);
  glBegin(GL_LINES);
  glVertex2f(parts[0][p1].x, parts[0][p1].y);
  glVertex2f(parts[0][p2].x, parts[0][p2].y);
  glEnd();
  glBegin(GL_POINTS);
  glColor3f(0, 0, 1);
  glVertex2f(parts[0][p1].x, parts[0][p1].y);
  glColor3f(0, 1, 0);
  glVertex2f(parts[0][p2].x, parts[0][p2].y);
  glEnd();
}

void Geometry::renderTriangles()
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_TRIANGLES);
  auto p = parts[0];
  for (auto t : triangles)
  {
    glVertex2f(p[t.v1].x, p[t.v1].y);
    glVertex2f(p[t.v2].x, p[t.v2].y);
    glVertex2f(p[t.v3].x, p[t.v3].y);
  }
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
