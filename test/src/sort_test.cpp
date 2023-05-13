#include "../../src/include/utils/algorithm.h"
// #include "../../src/include/container/stl/vector.h"

#include <bits/stdc++.h>

auto main() -> int {
  std::vector<int> vec;
  int range = 20;
  for (int i = 0; i < 10; i++) {
    vec.push_back(rand() % range);
    std::cout << vec.back() << ' ';
  }
  std::cout << '\n';
  std::sort(vec.begin(), vec.end(), [](int a, int b) { return a < b; });
  for (int i = 0; i < 10; i++) {
    std::cout << vec[i] << ' ';
  }
  std::cout << '\n';
  return 0;
}