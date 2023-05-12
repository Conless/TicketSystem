//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// macros.h
//
// Identification: src/include/common/macros.h
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <cassert>
#include <stdexcept>

namespace bustub {

#define BUSTUB_ASSERT(expr, message) assert((expr) && (message))

#define UNIMPLEMENTED(message) throw std::logic_error(message)

#define BUSTUB_ENSURE(expr, message) \
  if (!(expr)) {                     \
    throw std::logic_error(message); \
  }

#define UNREACHABLE(message) throw std::logic_error(message)

// Macros to disable copying and moving
#define DISALLOW_COPY(cname)                                    \
  cname(const cname &) = delete;                   /* NOLINT */ \
  auto operator=(const cname &)->cname & = delete; /* NOLINT */

#define DISALLOW_MOVE(cname)                               \
  cname(cname &&) = delete;                   /* NOLINT */ \
  auto operator=(cname &&)->cname & = delete; /* NOLINT */

#define DISALLOW_COPY_AND_MOVE(cname) \
  DISALLOW_COPY(cname);               \
  DISALLOW_MOVE(cname);

#ifdef CUSTOMIZED_BUSTUB

#define BUSTUB_DECLARE(TypeName)                                                                                   \
  namespace bustub {                                                                                               \
  template class TypeName<StandardKey<int>, int, StandardKey<int>::Comparator>;                       /* NOLINT */ \
  template class TypeName<PairKey<StringKey<65>, int>, int, PairKey<StringKey<65>, int>::Comparator>; /* NOLINT */ \
  }

#define BUSTUB_NTS_DECLARE(TypeName)                                                                 \
  namespace bustub {                                                                                 \
  template class TypeName<StandardKey<int>, int, StandardKey<int>::Comparator, false>; /* NOLINT */  \
  template class TypeName<PairKey<StringKey<65>, int>, int, PairKey<StringKey<65>, int>::Comparator, \
                          false>; /* NOLINT */                                                       \
  }

#define BUSTUB_INTERNAL_DECLARE(TypeName)                                                                          \
  namespace bustub {                                                                                               \
  template class TypeName<StandardKey<int>, int, StandardKey<int>::Comparator>;                       /* NOLINT */ \
  template class TypeName<PairKey<StringKey<65>, int>, int, PairKey<StringKey<65>, int>::Comparator>; /* NOLINT */ \
  }

#endif

}  // namespace bustub
