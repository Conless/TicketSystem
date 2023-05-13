#ifndef TRAIN_TYPE_H
#define TRAIN_TYPE_H

#include "container/types.h"
#include "user/user_type.h"

namespace conless {

using TrainID = StringKey<20>;
using StationID = StringKey<30>;
using TrainDateID = PairKey<TrainID, int>;
using TrainStationID = PairKey<StationID, TrainID>;
using TicketID = PairKey<UserName, int>;

constexpr int STATION_NUM_MAX = 100;
constexpr int WAITLIST_LENGTH = 100;

constexpr int TIME_MAX_IN_DAY = 24 * 60;

struct TrainInfo {
  TrainID train_id_;
  int seat_num_;
  int station_num_;

  StationID station_id_[STATION_NUM_MAX];
  int prices_[STATION_NUM_MAX];
  int start_time_;
  int dep_time_[STATION_NUM_MAX];
  int arr_time_[STATION_NUM_MAX];
  
  int start_date_;
  int end_date_;
  char type_;
};

struct TrainDateInfo { // 5248 bytes
  TrainID train_id_;
  int date_;
  int remain_ticket_[STATION_NUM_MAX];

  TicketID waitlist_[WAITLIST_LENGTH];
  int head_{0}, tail_{-1};
};

struct TrainStationInfo { // 80 bytes
  TrainID train_id_;
  StationID station_id_;
  int index_in_train_;
};

struct TicketInfo {
  TicketID ticket_id_;
  bool status_; // false for still waiting
  TrainID train_id_;
  int date_;
  int quantity_;
  int price_;
};

// Convert a date between int and std::string
auto date_to_int(const std::string &date) -> int; // NOLINT
auto date_to_string(int date) -> std::string;     // NOLINT

// Convert a time between int and std::string
auto time_to_int(const std::string &time) -> int; // NOLINT
auto time_to_string(int time) -> std::string;     // NOLINT

auto to_string(const TrainInfo &train_info, const TrainDateInfo &train_date_info) -> std::string; // NOLINT

}  // namespace conless

#endif