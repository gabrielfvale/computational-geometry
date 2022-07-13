#include <algorithm>
#include <istream>

#include "demo.h"
#include "hullbuffer.h"

Demo::Demo(const vector<vector<Point>> &p)
{
  calc_boundary(p);
}

void Demo::calc_boundary(const vector<vector<Point>> &p)
{
  if (p.size() >= 1)
  {
    auto part_used = p[0];
    for (auto point : part_used)
    {
      points.push_back(point);
    }
    boundary = HullBuffer::index_buffer(points);
  }
}

void Demo::triangulate(int step)
{
  if (step == -1)
  {
    return;
  }
  if (boundary.size() < 3)
  {
    return;
  }

  if (boundary.size() == 3 && triangles.size() == 0)
  {
    triangles.push_back(Triangle(boundary[0], boundary[1], boundary[2]));
  }

  int iteration = 0;
  int edge = 0;

  vector<int> iter_boundary;
  for (auto p : boundary)
  {
    iter_boundary.push_back(p);
  }

  // Update boundary checking for colinear points
  for (size_t i = 0; i < points.size(); ++i)
  {
    for (size_t j = 0; j < iter_boundary.size(); ++j)
    {
      int j_next = (j + 1) % iter_boundary.size();
      if (i != iter_boundary[j] && i != iter_boundary[j_next])
      {

        Edge e = Edge(iter_boundary[j], iter_boundary[j_next]);
        double dist = e.pDist(i, points);
        if (e.pLiesInByDist(i, points, 1))
        {
          iter_boundary.insert(iter_boundary.begin() + j_next, i);
          i = 0;
          break;
        }
      }
    }
  }

  while (iter_boundary.size() >= 3 && iteration < 200 && steps + 1 < step)
  {

    if (edge >= boundary.size() - 1)
    {
      edge = 0;
    }

    int edge_n = (edge + 1) % iter_boundary.size();
    ++iteration;

    int chosen_point = points.size() - 1;

    while (chosen_point == iter_boundary[edge] || chosen_point == iter_boundary[edge_n])
    {
      chosen_point = (chosen_point - 1) % points.size();
    }

    Edge h;
    Circle c = Circle(points[boundary[edge]], points[boundary[edge_n]], points[chosen_point]);
    Triangle t = Triangle(boundary[edge], boundary[edge_n], chosen_point);
    while (chosen_point >= 0)
    {
      if (chosen_point == boundary[edge] || chosen_point == boundary[edge_n])
      {
        chosen_point--;
        continue;
      }

      // Continue if points are colinear
      if (h.orientationf(boundary[edge], boundary[edge_n], chosen_point, points) != 1)
      {
        // output << "Points not in clockwise order" << endl;
        chosen_point--;
        continue;
      }

      c = Circle(points[boundary[edge]], points[boundary[edge_n]], points[chosen_point]);
      int in_circle = points.size() - 1;
      while (in_circle >= 0)
      {
        if (in_circle == chosen_point || in_circle == boundary[edge] || in_circle == boundary[edge_n])
        {
          in_circle--;
          continue;
        }
        if (c.inCircle(points[in_circle]))
        {
          break;
        }
        in_circle--;
      }

      if (in_circle < 0 && chosen_point != boundary[edge] && chosen_point != boundary[edge_n])
      {
        // output << "Circle is empty" << endl;
        t = Triangle(boundary[edge], boundary[edge_n], chosen_point);
        // output << t << endl;
        bool intersects = t.intersectsList(triangles, points);
        if (!t.intersectsList(triangles, points))
        {
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

    bool point_in_iter_boundary = false;
    for (auto i : iter_boundary)
    {
      if (i == chosen_point)
      {
        point_in_iter_boundary = true;
        break;
      }
    }
    // cout << "Point in iter_boundary: " << point_in_iter_boundary << endl;
    if (point_in_iter_boundary)
    {
      int prev = HullBuffer::getIndex(iter_boundary.size(), edge - 1);
      int next = HullBuffer::getIndex(iter_boundary.size(), edge_n + 1);
      if (iter_boundary[prev] == chosen_point)
      {
        iter_boundary.erase(iter_boundary.begin() + edge);
      }
      else if (iter_boundary[next] == chosen_point)
      {
        iter_boundary.erase(iter_boundary.begin() + edge_n);
      }
      else
      {
        // its a loop, continue.
        if (chosen_point != iter_boundary[next])
        {
          // cout << "INVALID LOOP! CHOSEN NOT NEXT" << endl;
          edge++;
          continue;
        }
        else
        {
          iter_boundary.insert(iter_boundary.begin() + edge_n, chosen_point);
        }
      }
    }
    else
    {
      iter_boundary.insert(iter_boundary.begin() + edge_n, chosen_point);
    }

    triangles.push_back(t);
    edge = 0;
    steps++;
  }

  if (iter_boundary.size() == 3)
  {
    Triangle t = Triangle(iter_boundary[0], iter_boundary[1], iter_boundary[2]);
    if (!t.pointsInTriangle(points))
    {
      triangles.push_back(t);
    }
    iter_boundary.clear();
  }

  boundary = iter_boundary;
}

void Demo::render()
{
  glColor3f(1, 0.5, 0.3);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_TRIANGLES);
  for (auto t : triangles)
  {
    glVertex2f(points[t.v1].x, points[t.v1].y);
    glVertex2f(points[t.v2].x, points[t.v2].y);
    glVertex2f(points[t.v3].x, points[t.v3].y);
  }
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnd();

  glColor3f(1, 1, 1);
  glBegin(GL_LINES);
  for (size_t j = 0; j < boundary.size(); ++j)
  {
    int next = (j + 1) % boundary.size();
    glVertex2f(points[boundary[j]].x, points[boundary[j]].y);
    glVertex2f(points[boundary[next]].x, points[boundary[next]].y);
  }
  glEnd();

  glColor3f(0, 0, 1);
  glBegin(GL_LINES);
  glVertex2f(points[selected_edge.first].x, points[selected_edge.first].y);
  glVertex2f(points[selected_edge.second].x, points[selected_edge.second].y);
  glEnd();
}

void Demo::renderDebug(int p1, int p2, int p)
{
  // edge is red
  // first is blue, second is green,

  // glColor3f(1, 0, 0);
  glBegin(GL_LINES);
  glVertex2f(points[p1].x, points[p1].y);
  glVertex2f(points[p2].x, points[p2].y);
  glEnd();
  glBegin(GL_POINTS);
  glColor3f(0, 0, 1);
  glVertex2f(points[p1].x, points[p1].y);
  glColor3f(0, 1, 0);
  glVertex2f(points[p2].x, points[p2].y);
  glEnd();
}
