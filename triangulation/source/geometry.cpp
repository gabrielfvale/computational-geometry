#include "geometry.h"
#include <algorithm>
#include <istream>

#include "hullbuffer.h"

using namespace std;

Geometry::Geometry() {}

Geometry::Geometry(const vector<vector<Point>> &p)
{
  for (auto v : p)
  {
    parts.push_back(v);
  }
}

Geometry::~Geometry()
{
  parts.clear();
  hulls.clear();
  triangles.clear();
}

void Geometry::calc_hulls()
{
  for (size_t i = 0; i < parts.size(); ++i)
  {
    vector<int> buffer = HullBuffer::index_buffer(parts[i]);
    cout << "Buffer size: " << buffer.size() << endl;
    hulls.push_back(buffer);
  }
}

void Geometry::triangulate()
{
  // int k = 0;
  for (size_t k = 0; k < hulls.size(); ++k)
  {
    cout << "Iterating current hull: " << k << ", " << hulls[k].size() << endl;
    if (hulls[k].size() < 3)
    {
      continue;
    }

    vector<int> boundary;
    for (auto i : hulls[k])
      boundary.push_back(i);

    vector<Point> available;
    for (auto p : parts[k])
      available.push_back(p);

    vector<int> left;
    for (size_t i = 0; i < parts[k].size(); ++i)
      left.push_back(i);

    vector<Triangle> iter_triangles;

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

    while (boundary.size() > 3 && iteration <= 200)
    {
      cout << endl;
      cout << "Iteration: " << iteration << endl;
      cout << "Boundary is: ";
      for (auto p : boundary)
        cout << p << ", ";
      cout << endl;

      if (edge >= boundary.size() - 1)
      {
        edge = 0;
      }

      int edge_n = (edge + 1) % boundary.size();
      cout << "Edge " << edge << ": " << boundary[edge] << ", " << boundary[edge_n] << endl;
      cout << "Edge P: " << available[boundary[edge]] << ", " << available[boundary[edge_n]] << endl;
      ++iteration;

      int chosen_point = available.size() - 1;

      while (chosen_point == boundary[edge] || chosen_point == boundary[edge_n])
      {
        chosen_point = (chosen_point - 1) % available.size();
      }

      Edge h;
      Circle c = Circle(available[boundary[edge]], available[boundary[edge_n]], available[chosen_point]);
      Triangle t = Triangle(boundary[edge], boundary[edge_n], chosen_point);
      // c.inCircle(available[0]);
      bool circle_empty = false;
      while (chosen_point >= 0)
      {
        cout << "Checking point " << chosen_point << ", " << available[chosen_point] << endl;
        if (chosen_point == boundary[edge] || chosen_point == boundary[edge_n])
        {
          chosen_point--;
          continue;
        }

        // Continue if points are colinear
        if (h.orientation(boundary[edge], boundary[edge_n], chosen_point, parts[k]) != 1)
        {
          // cout << "Points are colinear" << endl;
          chosen_point--;
          continue;
        }

        c = Circle(available[boundary[edge]], available[boundary[edge_n]], available[chosen_point]);
        int in_circle = left.size() - 1;
        while (in_circle >= 0)
        {
          if (in_circle == chosen_point || in_circle == boundary[edge] || in_circle == boundary[edge_n])
          {
            // cout << "P index is same as chosen" << endl;
            in_circle--;
            continue;
          }
          if (c.inCircle(available[in_circle]))
          {
            cout << "a point is in this circle:" << endl;
            cout << "I, Chosen, C, R: "
                 << in_circle << " "
                 << available[chosen_point] << " "
                 << c.center << " "
                 << c.radius << " "
                 << endl;
            cout << available[in_circle] << endl;
            cout << endl;
            break;
          }
          in_circle--;
        }

        if (in_circle < 0 && chosen_point != boundary[edge] && chosen_point != boundary[edge_n])
        {
          cout << "Circle is empty" << endl;
          t = Triangle(boundary[edge], boundary[edge_n], chosen_point);
          // cout << t << endl;
          if (!t.intersectsList(iter_triangles, available))
          {
            circle_empty = true;
            cout << "Found a circle and triangle combo: " << t << endl;
            cout << available[chosen_point] << endl;
            break;
          }
        }
        chosen_point--;
      }

      if (chosen_point < 0)
      {
        edge++;
        continue;
      }

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
          // left.erase(left.begin() + left_index);
          // cout << "Left is: ";
          // for (auto p : left)
          //   cout << p << ", ";
          // cout << endl;
        }
        else if (boundary[next] == chosen_point)
        {
          cout << "Point is next" << endl;
          boundary.erase(boundary.begin() + edge_n);
          // left.erase(left.begin() + boundary[edge_n]);
          // cout << "Left is: ";
          // for (auto p : left)
          //   cout << p << ", ";
          // cout << endl;
        }
        else
        {
          // its a loop, continue.
          cout << "Point Extra case" << endl;
          // Edge h;
          // if (helper.orientation(boundary[edge], boundary[edge_n], boundary[next], available) == 0)
          // {
          //   cout << "BOUNDARY LOOP" << endl;
          // }

          cout << "This is chosen: " << chosen_point << " and this is next: " << next << endl;

          if (chosen_point != boundary[next])
          {
            cout << "INVALID LOOP! CHOSEN NOT NEXT" << endl;
            edge++;
            continue;
          }
          else
          {
            boundary.erase(boundary.begin() + edge_n);
            left.erase(left.begin() + boundary[edge_n]);
          }
        }
      }
      else
      {
        boundary.insert(boundary.begin() + edge_n, chosen_point);
      }

      cout << "<-- INSERTING TRIANGLE --> " << endl;
      cout << t << endl;
      iter_triangles.push_back(t);
      // edge = 0;
    }

    if (boundary.size() == 3)
    {
      Triangle t = Triangle(boundary[0], boundary[1], boundary[2]);
      if (!t.pointsInTriangle(available))
      {
        iter_triangles.push_back(t);
      }
      boundary = {};
    }

    // hulls[k] = boundary;
    triangles.push_back(iter_triangles);
  }
}

void Geometry::renderHulls()
{
  glBegin(GL_LINES);
  for (size_t i = 0; i < hulls.size(); ++i)
  {
    auto buffer = hulls[i];
    auto part = parts[i];
    for (size_t j = 0; j < buffer.size(); ++j)
    {
      int next = (j + 1) % buffer.size();
      glVertex2f(part[buffer[j]].x, part[buffer[j]].y);
      glVertex2f(part[buffer[next]].x, part[buffer[next]].y);
    }
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

void Geometry::renderDebug(int v1, int v2, int p)
{
  int prev = HullBuffer::getIndex(hulls[0].size(), v1 - 1);
  int next = HullBuffer::getIndex(hulls[0].size(), v2 + 1);
  int next_p = HullBuffer::getIndex(hulls[0].size(), p + 1);

  int c_v1 = hulls[0][v1];
  int c_v2 = hulls[0][v2];
  int c_p = hulls[0][p];
  // Current edge
  glColor3f(1, 0, 0);
  renderDebugEdge(c_v1, c_v2);
  // Next edge
  glColor3f(1, 1, 0);
  renderDebugEdge(c_v2, hulls[0][next]);
  // glColor3f(1, 1, 0);
  // renderDebugEdge(c_p, hulls[0][next_p]);
  // Prev edge
  glColor3f(0, 1, 1);
  renderDebugEdge(hulls[0][prev], c_v1);
  // Final triangle
  // glColor3f(1, 1, 1);
  // renderDebugEdge(c_p, c_v2);
}

void Geometry::renderTriangles()
{
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // glBegin(GL_TRIANGLES);
  // auto p = parts[0];
  // for (auto t : triangles)
  // {
  //   glVertex2f(p[t.v1].x, p[t.v1].y);
  //   glVertex2f(p[t.v2].x, p[t.v2].y);
  //   glVertex2f(p[t.v3].x, p[t.v3].y);
  // }
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // glEnd();

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_TRIANGLES);

  for (size_t i = 0; i < triangles.size(); ++i)
  {
    auto part = triangles[i];
    auto p = parts[i];
    for (auto t : part)
    {
      glVertex2f(p[t.v1].x, p[t.v1].y);
      glVertex2f(p[t.v2].x, p[t.v2].y);
      glVertex2f(p[t.v3].x, p[t.v3].y);
    }
  }
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnd();
}

void Geometry::renderPolygon()
{
  glBegin(GL_POLYGON);

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
