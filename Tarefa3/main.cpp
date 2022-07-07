#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <math.h>

#include "graham.h"
#include "jarvis.h"
#include "quickhull.h"
#include "mergehull.h"
#include "Point.h"

using namespace std;

vector<Point> randomVector(int count, double minimum, double maximum)
{
  random_device rd;
  mt19937 gen(rd());

  vector<Point> values(count);
  uniform_real_distribution<> dis(minimum, maximum);
  generate(values.begin(), values.end(), [&]()
           { Point p; p.x=dis(gen); p.y=dis(gen); return p; });
  return values;
}

void printhull(vector<Point> const &hull)
{
  for (auto p : hull)
  {
    cout << "(" << p.x << ", "
         << p.y << ")" << endl;
  }
  cout << endl;
}

int main()
{

  vector<Point> example = {
      {-2.0, 0},
      {0, 0},
      {2.0, 0},
      {3, 1},
      {1.5, 1.5},
      {0.5, 2.2},
      {0.5, 3.4},
      {-2.2, 3.4},
      {-1.8, 2.1},
      {-3.2, 1.7}};

  // Use sorting algorithms
  vector<Point> graham_res = graham(example, example.size());
  vector<Point> jarvis_res = jarvis(example, example.size());
  vector<Point> quick_res = quickHull(example, example.size());
  vector<Point> merge_res = mergeHull(example);

  cout << "Graham:" << endl;
  printhull(graham_res);
  cout << endl;

  cout << "Jarvis:" << endl;
  printhull(jarvis_res);
  cout << endl;

  cout << "Quickhull:" << endl;
  printhull(quick_res);
  cout << endl;

  cout << "Mergehull:" << endl;
  printhull(merge_res);
  cout << endl;

  for (int p = 1; p <= 3; ++p)
  {
    // Get count from power
    int count = pow(10, p);
    // Create random vector
    vector<Point> randVec = randomVector(count, -20.0, 20.0);

    // Print results
    // cout
    //     << "USING " << count << " VALUES" << endl;
  }

  return 0;
}
