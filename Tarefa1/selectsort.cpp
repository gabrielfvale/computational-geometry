#include "selectsort.h"

#include <vector>

void selectSort(std::vector<double> &v)
{
  int s = v.size();

  for (int i = 0; i < s; ++i)
  {
    int m = i;
    for (int j = i; j < s; ++j)
    {
      if (v[j] < v[m])
      {
        m = j;
      }
    }
    double tmp = v[i];
    v[i] = v[m];
    v[m] = tmp;
  }
}
