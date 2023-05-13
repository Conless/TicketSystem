#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <iostream>
#include <utility>

namespace conless {

template <typename Iterator, typename Compare>
void sort(Iterator first, Iterator last, Compare comp) {  // NOLINT
  auto low = first;
  auto high = last - 1;
  if (low - high >= 0) {
    return;
  }
  auto temp = *first;
  while (low != high) {
    while (!comp(*high, temp) && high - low > 0) {
      high--;
    }
    while (!comp(temp, *low) && high - low > 0) {
      low++;
    }
    if (high - low > 0) {
      std::swap(*low, *high);
    }
  }

  std::swap(*first, *low);
  conless::sort(first, low, comp);
  conless::sort(high + 1, last, comp);
}

template <typename Iterator>
void sort(Iterator first, Iterator last) {  // NOLINT
  auto low = first;
  auto high = last - 1;
  if (low - high >= 0) {
    return;
  }
  auto temp = *first;
  while (low != high) {
    while (*high >= temp && high - low > 0) {
      high--;
    }
    while (temp >= *low && high - low > 0) {
      low++;
    }
    if (high - low > 0) {
      std::swap(*low, *high);
    }
  }

  std::swap(*first, *low);
  conless::sort(first, low);
  conless::sort(high + 1, last);
}

}  // namespace conless

#endif