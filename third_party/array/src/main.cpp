#include <bits/stdc++.h>
#include "array.h"

using namespace std; // NOLINT
using namespace conless; // NOLINT

auto main() -> int {
  Array<int> arr("data.db");
  int t;
  cin >> t;
  for (int i = 0; i < t; i++) {
    int num;
    cin >> num;
    arr.Insert(num);
  }
  for (int i = 0; i < t; i++) {
    int index;
    cin >> index;
    cout << arr.ValueAt(index) << '\n';
  }
  cout << endl;
  return 0;
}