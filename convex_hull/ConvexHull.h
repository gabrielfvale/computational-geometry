#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
#include <tuple>

#include <SDL2/SDL.h>

SDL_FPoint p0;
SDL_FPoint second_top(std::stack<SDL_FPoint> &s)
{
  SDL_FPoint temp_point = s.top();
  s.pop();
  SDL_FPoint res = s.top();
  s.push(temp_point);
  return res;
}

void swap(SDL_FPoint &p1, SDL_FPoint &p2)
{
  SDL_FPoint temp = p1;
  p1 = p2;
  p2 = temp;
}

int squared_dist(SDL_FPoint p1, SDL_FPoint p2)
{
  return ((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

int orientation(SDL_FPoint a, SDL_FPoint b, SDL_FPoint c)
{
  int value = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
  // Colinear
  if (value == 0)
    return 0;
  // CW or CCW, respectively
  return (value > 0) ? 1 : 2;
}

int comp(const void *point1, const void *point2)
{
  SDL_FPoint *p1 = (SDL_FPoint *)point1;
  SDL_FPoint *p2 = (SDL_FPoint *)point2;
  int ori = orientation(p0, *p1, *p2);
  if (ori == 0)
    return (squared_dist(p0, *p2) >= squared_dist(p0, *p1)) ? -1 : 1;
  return (ori == 2) ? -1 : 1;
}

static std::tuple<SDL_FPoint *, int> convex_hull(SDL_FPoint points[], int size)
{
  std::vector<SDL_FPoint> convex_hull_points;

  // Iterate over points to find bottom-most or left-most
  int min_y = points[0].y, min = 0;
  for (int i = 1; i < size; ++i)
  {
    int y = points[i].y;
    if ((y < min_y) || (min_y == y) && points[i].x < points[min].x)
    {
      min_y = points[i].y;
      min = i;
    }
  }

  // Swap min point
  swap(points[0], points[min]);

  // Sort
  p0 = points[0];
  std::qsort(&points[1], size - 1, sizeof(SDL_FPoint), comp);

  int n = 1;

  for (int i = 1; i < size; ++i)
  {
    while (i < n - 1 && orientation(p0, points[i], points[i + 1]) == 0)
      i++;
    points[n] = points[i];
    n++;
  }

  // Case when convex hull is not possible
  if (n < 3)
    return std::make_tuple(nullptr, 0);

  // Otherwise continue
  std::stack<SDL_FPoint> s;
  s.push(points[0]);
  s.push(points[1]);
  s.push(points[2]);

  for (int i = 3; i < n; ++i)
  {
    while (s.size() > 1 && orientation(second_top(s), s.top(), points[i]) != 2)
      s.pop();
    s.push(points[i]);
  }

  std::vector<SDL_FPoint> v;
  while (!s.empty())
  {
    SDL_FPoint p = s.top();
    v.push_back(p);
    s.pop();
  }

  SDL_FPoint *result = new SDL_FPoint[v.size()];
  memcpy(result, &v.front(), v.size() * sizeof(SDL_FPoint));

  return std::make_tuple(result, v.size());
}

static std::vector<SDL_FPoint> convex_hull2(std::vector<SDL_FPoint> input)
{
  std::vector<SDL_FPoint> convex_hull_points;

  SDL_FPoint *points = new SDL_FPoint[input.size()];
  memcpy(points, &input.front(), input.size() * sizeof(SDL_FPoint));
  int size = input.size();

  // Iterate over points to find bottom-most or left-most
  int min_y = points[0].y, min = 0;
  for (int i = 1; i < size; ++i)
  {
    int y = points[i].y;
    if ((y < min_y) || (min_y == y) && points[i].x < points[min].x)
    {
      min_y = points[i].y;
      min = i;
    }
  }

  // Swap min point
  swap(points[0], points[min]);

  // Sort
  p0 = points[0];
  std::qsort(&points[1], size - 1, sizeof(SDL_FPoint), comp);

  int n = 1;

  for (int i = 1; i < size; ++i)
  {
    while (i < n - 1 && orientation(p0, points[i], points[i + 1]) == 0)
      i++;
    points[n] = points[i];
    n++;
  }

  // Case when convex hull is not possible
  if (n < 3)
    return {};

  // Otherwise continue
  std::stack<SDL_FPoint> s;
  s.push(points[0]);
  s.push(points[1]);
  s.push(points[2]);

  for (int i = 3; i < n; ++i)
  {
    while (s.size() > 1 && orientation(second_top(s), s.top(), points[i]) != 2)
      s.pop();
    s.push(points[i]);
  }

  std::vector<SDL_FPoint> v;
  while (!s.empty())
  {
    SDL_FPoint p = s.top();
    v.push_back(p);
    s.pop();
  }

  return v;
}

static void merge_points(std::vector<SDL_FPoint> &points, std::vector<SDL_FPoint> to_merge, double eps = 0.001)
{
  for (int i = 0; i < to_merge.size(); ++i)
  {
    bool exists = false;
    int j = 0;
    while (!exists && j < points.size())
    {
      if (i != j)
      {
        exists = squared_dist(to_merge[i], points[j]) <= eps;
      }
      ++j;
    }
    if (!exists)
    {
      points.push_back(to_merge[i]);
    }
  }
}

std::vector<std::vector<SDL_FPoint>> joined_convex_hull(std::vector<std::vector<SDL_FPoint>> object)
{
  std::vector<std::vector<SDL_FPoint>> hulls = {};
  for (auto part : object)
  {
    auto hull = convex_hull2(part);
    // merge_points(points, hull);
    hulls.push_back(hull);
  }
  return hulls;
}

std::vector<SDL_FPoint> hull_cloud(std::vector<std::vector<SDL_FPoint>> object)
{
  std::vector<SDL_FPoint> points = {};
  for (auto part : object)
  {
    auto hull = convex_hull2(part);
    merge_points(points, hull);
  }
  return points;
}
