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

  int start_time_;
  int start_date_;
  int end_date_;
  char type_;
  bool released_{false};

  StationID stations_id_[STATION_NUM_MAX];
  int prices_[STATION_NUM_MAX]; // the total price from the first station to the i-th station, 0-base
  int arr_times_[STATION_NUM_MAX]; // the arrival time of each station, 1-base from the second station to the last station
  int dep_times_[STATION_NUM_MAX]; // the departure time of each station, 0-base from the first station to the last but one
};

struct TrainDateInfo { // 5248 bytes
  TrainID train_id_;
  int date_;
  int remain_tickets_[STATION_NUM_MAX]; // the remain ticket from the i-th station to the i+1-th, 0-base from the first

  TicketID waitlist_[WAITLIST_LENGTH];
  int head_{0}, tail_{-1};
};

struct TrainStationInfo { // 80 bytes
  TrainID train_id_;
  StationID stations_id_;
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

auto to_string(const TrainInfo &train_info, const TrainDateInfo &train_date_info) -> std::string;  // NOLINT
auto to_string(const TrainInfo &train_info, int date) -> std::string;                              // NOLINT
auto to_string(const TrainInfo &train_info, const TrainDateInfo &train_date_info, int date, int start_index, int dest_index) -> std::string; // NOLINT

}  // namespace conless

#endif