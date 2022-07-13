#include <algorithm>
#include <istream>

#include "demo.h"
#include "hullbuffer.h"

Demo::Demo(const vector<vector<Point>> &p)
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

  while (iter_boundary.size() >= 3 && iteration < 500 && steps + 1 < step)
  {
    // cout << endl;
    // cout << "Iteration: " << iteration << endl;
    // cout << "Boundary is: ";
    // for (auto p : iter_boundary)
    //   cout << p << ", ";
    // cout << endl;

    int edge_n = (edge + 1) % iter_boundary.size();
    // cout << "Edge " << edge << ": " << iter_boundary[edge] << ", " << iter_boundary[edge_n] << endl;
    ++iteration;
    if (edge >= iter_boundary.size() - 1)
    {
      edge = 0;
    }

    int chosen_point = points.size() - 1;

    while (chosen_point == iter_boundary[edge] || chosen_point == iter_boundary[edge_n])
    {
      chosen_point = (chosen_point - 1) % points.size();
    }

    auto t = Triangle(iter_boundary[edge], iter_boundary[edge_n], chosen_point);
    Edge helper;
    // cout << "P, I: " << t.pointsInTriangle(points) << ", " << t.intersectsList(triangles, points) << endl;
    while (
        chosen_point >= 0 &&
        (helper.orientation(iter_boundary[edge], iter_boundary[edge_n], chosen_point, points) == 0 ||
         (t.pointsInTriangle(points) || t.intersectsList(triangles, points))))
    {
      chosen_point--;
      // cout << iter_boundary[edge] << ", " << iter_boundary[edge_n] << ", " << chosen_point << endl;
      if (chosen_point == iter_boundary[edge] || chosen_point == iter_boundary[edge_n])
      {
        continue;
      }
      t = Triangle(iter_boundary[edge], iter_boundary[edge_n], chosen_point);
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
      // cout << "Prev, next: " << iter_boundary[prev] << ", " << iter_boundary[next] << endl;
      // cout << "Chosen: " << chosen_point << endl;
      if (iter_boundary[prev] == chosen_point)
      {
        // cout << "Point is prev" << endl;
        iter_boundary.erase(iter_boundary.begin() + edge);
      }
      else if (iter_boundary[next] == chosen_point)
      {
        // cout << "Point is next" << endl;
        iter_boundary.erase(iter_boundary.begin() + edge_n);
      }
      else
      {
        // its a loop, continue.
        // cout << "Point Extra case" << endl;
        // Edge h;
        // if (helper.orientation(iter_boundary[edge], iter_boundary[edge_n], iter_boundary[next], points) == 0)
        // {
        //   cout << "iter_boundary LOOP" << endl;
        // }

        if (chosen_point != next)
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

    // cout << "<-- INSERTING TRIANGLE --> " << endl;
    // cout << t << endl;
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
  }

  boundary = iter_boundary;
}

void Demo::render()
{
  glColor3f(1, 1, 1);
  glBegin(GL_LINES);
  for (size_t j = 0; j < boundary.size(); ++j)
  {
    int next = (j + 1) % boundary.size();
    glVertex2f(points[boundary[j]].x, points[boundary[j]].y);
    glVertex2f(points[boundary[next]].x, points[boundary[next]].y);
  }
  glEnd();

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
