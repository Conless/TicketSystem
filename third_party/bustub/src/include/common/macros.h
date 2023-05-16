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
  using namespace conless;                                                                            /* NOLINT */ \
  template class TypeName<StandardKey<int>, int, StandardKey<int>::Comparator>;                       /* NOLINT */ \
  template class TypeName<PairKey<StringKey<65>, int>, int, PairKey<StringKey<65>, int>::Comparator>; /* NOLINT */ \
  template class TypeName<UserName, UserInfo, UserName::Comparator>;                                  /* NOLINT */ \
  template class TypeName<TrainID, TrainInfo, TrainID::Comparator>;                                   /* NOLINT*/  \
  template class TypeName<TrainDateID, TrainDateInfo, TrainDateID::Comparator>;                       /* NOLINT*/  \
  template class TypeName<TrainStationID, TrainStationInfo, TrainStationID::Comparator>;              /* NOLINT*/  \
  template class TypeName<TicketUserInfo, TicketInfo, TicketUserInfo::Comparator>;                                /* NOLINT*/  \
  }  // namespace bustub

#define BUSTUB_NTS_DECLARE(TypeName)                                                                              \
  namespace bustub {                                                                                              \
  using namespace conless;                                                                           /* NOLINT */ \
  template class TypeName<StandardKey<int>, int, StandardKey<int>::Comparator, false>;               /* NOLINT */ \
  template class TypeName<PairKey<StringKey<65>, int>, int, PairKey<StringKey<65>, int>::Comparator, /* NOLINT */ \
                          false>;                                                                    /* NOLINT */ \
  template class TypeName<UserName, UserInfo, UserName::Comparator, false>;                          /* NOLINT */ \
  template class TypeName<TrainID, TrainInfo, TrainID::Comparator, false>;                           /* NOLINT*/  \
  template class TypeName<TrainDateID, TrainDateInfo, TrainDateID::Comparator, false>;               /* NOLINT*/  \
  template class TypeName<TrainStationID, TrainStationInfo, TrainStationID::Comparator, false>;      /* NOLINT*/  \
  template class TypeName<TicketUserInfo, TicketInfo, TicketUserInfo::Comparator, false>;                        /* NOLINT*/  \
  }                                                                                                  // namespace bustub

#define BUSTUB_INTERNAL_DECLARE(TypeName)                                                                          \
  namespace bustub {                                                                                               \
  using namespace conless;                                                                            /* NOLINT */ \
  template class TypeName<StandardKey<int>, int, StandardKey<int>::Comparator>;                       /* NOLINT */ \
  template class TypeName<PairKey<StringKey<65>, int>, int, PairKey<StringKey<65>, int>::Comparator>; /* NOLINT */ \
  template class TypeName<StringKey<20>, int, StringKey<20>::Comparator>;                             /* NOLINT */ \
  template class TypeName<PairKey<StringKey<20>, int>, int, PairKey<StringKey<20>, int>::Comparator>; /* NOLINT*/  \
  template class TypeName<PairKey<StringKey<30>, StringKey<20>>, int,                                 /* NOLINT*/  \
                          PairKey<StringKey<30>, StringKey<20>>::Comparator>;                         /* NOLINT*/  \
  }  // namespace bustub

#endif

}  // namespace bustub
