#include "mergesort.h"

#include <vector>

using namespace std;

void finalMerge(vector<double> &v, int s, int m, int e)
{
  vector<double> temp;

  int i, j;
  i = s;
  j = m + 1;

  while (i <= m && j <= e)
  {

    if (v[i] <= v[j])
    {
      temp.push_back(v[i]);
      ++i;
    }
    else
    {
      temp.push_back(v[j]);
      ++j;
    }
  }

  while (i <= m)
  {
    temp.push_back(v[i]);
    ++i;
  }

  while (j <= e)
  {
    temp.push_back(v[j]);
    ++j;
  }

  for (int i = s; i <= e; ++i)
    v[i] = temp[i - s];
}

void mergeSort(vector<double> &v, int s, int r)
{
  if (s < r)
  {
    int m = (s + r) / 2;
    mergeSort(v, s, m);
    mergeSort(v, m + 1, r);
    finalMerge(v, s, m, r);
  }
}
