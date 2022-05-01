#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <math.h>

#include "selectsort.h"
#include "quicksort.h"
#include "mergesort.h"

using namespace std;

template <typename T>
vector<T> randomVector(int count, T minimum, T maximum)
{
  random_device rd;
  mt19937 gen(rd());

  vector<T> values(count);
  uniform_real_distribution<> dis(minimum, maximum);
  generate(values.begin(), values.end(), [&]()
           { return dis(gen); });
  return values;
}

template <typename T>
void printv(std::vector<T> const &v)
{
  for (auto i : v)
  {
    std::cout << i << ' ';
  }
  std::cout << std::endl;
}

int main()
{
  vector<double> sortedVec{-19.4683, -8.34057, -3.22413, -0.966957, 1.80666, 2.05719, 10.6336, 11.0487, 14.1233, 15.0202};
  // Create vector copies
  auto sortedVec_copy1 = sortedVec;
  auto sortedVec_copy2 = sortedVec;
  auto sortedVec_copy3 = sortedVec;

  // Use sorting algorithms
  selectSort(sortedVec_copy1);
  quickSort(sortedVec_copy2, 0, sortedVec_copy2.size() - 1);
  mergeSort(sortedVec_copy3, 0, sortedVec_copy3.size() - 1);

  // Print results
  cout << "USING SORTED VECTOR" << endl;
  cout << "Original vector:" << endl;
  printv(sortedVec);
  cout << endl;

  cout << "selectSort result:" << endl;
  printv(sortedVec_copy1);
  cout << endl;

  cout << "quickSort result:" << endl;
  printv(sortedVec_copy2);
  cout << endl;
  cout << "mergeSort result:" << endl;
  printv(sortedVec_copy3);
  cout << endl
       << endl;

  for (int p = 1; p <= 3; ++p)
  {
    // Get count from power
    int count = pow(10, p);
    // Create random vector
    vector<double> randVec = randomVector(count, -20.0, 20.0);

    // Create vector copies
    auto randVec_copy1 = randVec;
    auto randVec_copy2 = randVec;
    auto randVec_copy3 = randVec;

    // Use sorting algorithms
    selectSort(randVec_copy1);
    quickSort(randVec_copy2, 0, randVec_copy2.size() - 1);
    mergeSort(randVec_copy3, 0, randVec_copy3.size() - 1);

    // Print results
    cout << "USING " << count << " VALUES" << endl;
    cout << "Original vector:" << endl;
    printv(randVec);
    cout << endl;

    cout << "selectSort result:" << endl;
    printv(randVec_copy1);
    cout << endl;

    cout << "quickSort result:" << endl;
    printv(randVec_copy2);
    cout << endl;
    cout << "mergeSort result:" << endl;
    printv(randVec_copy3);
    cout << endl
         << endl;
  }

  return 0;
}
