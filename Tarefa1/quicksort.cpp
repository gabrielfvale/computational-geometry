#include "quicksort.h"

#include <vector>

void quickSort(std::vector<double> &v, int s, int r)
{
  if (s < r)
  {
    int pivot = r;
    int p = s;
    for (int i = s; i < r; ++i)
    {
      if (v[i] < v[pivot])
      {
        std::swap(v[i], v[p]);
        ++p;
      }
    }
    std::swap(v[p], v[pivot]);
    quickSort(v, s, p - 1);
    quickSort(v, p + 1, r);
  }
}
