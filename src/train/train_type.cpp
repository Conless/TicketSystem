#include "train/train_type.h"
#include <string>

namespace conless {

/**
 * @brief Convert a date from std::string to int
 *
 * @param date
 * @return the date count from 06-01, in range [1, 92]
 */
auto date_to_int(const std::string &date) -> int {
  int month = std::stoi(date.substr(0, 2));
  int day = std::stoi(date.substr(3, 2));
  if (month < 6 || month > 8) {
    return -1;
  }
  if (month > 6) {
    day += 30;
  }
  if (month > 7) {
    day += 31;
  }
  return day;
}

/**
 * @brief Convert a date from int to std::string
 *
 * @param date
 * @return std::string
 */
auto date_to_string(int date) -> std::string {
  int month = 6;
  if (date > 30) {
    month++;
    date -= 30;
  }
  while (date > 31) {
    month++;
    date -= 31;
  }
  std::string res = "0" + std::string{static_cast<char>('0' + month)} + "-";
  if (date >= 10) {
    res += std::to_string(date);
  } else {
    res += "0" + std::string{static_cast<char>('0' + date)};
  }
  return res;
}

/**
 * @brief Convert a time from std::string to int
 *
 * @param time
 * @return the time from 00:00, in range [0, TIME_MAX_IN_DAY - 1]
 */
auto time_to_int(const std::string &time) -> int {
  int hour = std::stoi(time.substr(0, 2));
  int minute = std::stoi(time.substr(3, 2));
  return hour * 60 + minute;
}

/**
 * @brief Convert a time from int to std::string
 *
 * @param time
 * @return std::string
 */
auto time_to_string(int time) -> std::string {
  int hour = (time / 60) % 24;
  int minute = time % 60;
  std::string res;
  if (hour >= 10) {
    res += std::to_string(hour);
  } else {
    res += "0" + std::string{static_cast<char>('0' + hour)};
  }
  res += ":";
  if (minute >= 10) {
    res += std::to_string(minute);
  } else {
    res += "0" + std::string{static_cast<char>('0' + minute)};
  }
  return res;
}

/**
 * @brief Convert a {TrainInfo, TrainDateInfo} to std::string
 *
 * @param train_info
 * @param train_date_info
 * @return std::string
 */
auto to_string(const TrainInfo &train_info, const TrainDateInfo &train_date_info) -> std::string {
  std::string res = std::string{train_info.train_id_} + " " + train_info.type_;
  for (int i = 0; i < train_info.station_num_; i++) {
    res += "\n";
    std::string arr_time;
    if (i == 0) {
      arr_time = "xx-xx xx:xx";
    } else {
      int arr_min = train_info.arr_times_[i];
      int arr_date = train_date_info.date_ + arr_min / TIME_MAX_IN_DAY;
      arr_time = date_to_string(arr_date) + " " + time_to_string(arr_min % TIME_MAX_IN_DAY);
    }
    std::string dep_time;
    if (i == train_info.station_num_ - 1) {
      dep_time = "xx-xx xx:xx";
    } else {
      int dep_min = train_info.dep_times_[i];
      int dep_date = train_date_info.date_ + dep_min / TIME_MAX_IN_DAY;
      dep_time = date_to_string(dep_date) + " " + time_to_string(dep_min % TIME_MAX_IN_DAY);
    }
    res += std::string{train_info.stations_id_[i]} + " ";
    res += arr_time + " -> ";
    res += dep_time + " ";
    res += std::to_string(train_info.prices_[i]) + " ";
    if (i != train_info.station_num_ - 1) {
      res += std::to_string(train_date_info.remain_tickets_[i]);
    } else {
      res += "x";
    }
  }
  return res;
}
auto to_string(const TrainInfo &train_info, int date) -> std::string {
  std::string res = std::string{train_info.train_id_} + " " + train_info.type_;
  for (int i = 0; i < train_info.station_num_; i++) {
    res += "\n";
    std::string arr_time;
    if (i == 0) {
      arr_time = "xx-xx xx:xx";
    } else {
      int arr_min = train_info.arr_times_[i];
      int arr_date = date + arr_min / TIME_MAX_IN_DAY;
      arr_time = date_to_string(arr_date) + " " + time_to_string(arr_min % TIME_MAX_IN_DAY);
    }
    std::string dep_time;
    if (i == train_info.station_num_ - 1) {
      dep_time = "xx-xx xx:xx";
    } else {
      int dep_min = train_info.dep_times_[i];
      int dep_date = date + dep_min / TIME_MAX_IN_DAY;
      dep_time = date_to_string(dep_date) + " " + time_to_string(dep_min % TIME_MAX_IN_DAY);
    }
    res += std::string{train_info.stations_id_[i]} + " ";
    res += arr_time + " -> ";
    res += dep_time + " ";
    res += std::to_string(train_info.prices_[i]) + " ";
    if (i != train_info.station_num_ - 1) {
      res += std::to_string(train_info.seat_num_);
    } else {
      res += "x";
    }
  }
  return res;
}
auto to_string(const TrainInfo &train_info, const TrainDateInfo &train_date_info, int date, int start_index,
               int dest_index) -> std::string {
  std::string train_ticket_info;
  int start_dep_time = train_info.dep_times_[start_index];
  int dest_arr_time = train_info.arr_times_[dest_index];
  train_ticket_info += std::string{train_date_info.train_id_} + " ";
  train_ticket_info += std::string{train_info.stations_id_[start_index]} + " ";
  train_ticket_info += date_to_string(date + start_dep_time / TIME_MAX_IN_DAY) + " ";
  train_ticket_info += time_to_string(start_dep_time) + " -> ";
  train_ticket_info += std::string{train_info.stations_id_[dest_index]} + " ";
  train_ticket_info += date_to_string(date + dest_arr_time / TIME_MAX_IN_DAY) + " ";
  train_ticket_info += time_to_string(dest_arr_time) + " ";
  train_ticket_info += std::to_string(train_info.prices_[dest_index] - train_info.prices_[start_index]) + " ";

  int remain_ticket = train_info.seat_num_;
  for (int i = start_index; i < dest_index; i++) {
    remain_ticket = std::min(remain_ticket, train_date_info.remain_tickets_[i]);
  }
  train_ticket_info += std::to_string(remain_ticket);
  return train_ticket_info;
}

auto to_string(const TicketInfo &ticket_info) -> std::string {
  std::string res;
  if (ticket_info.status_ == 1) {
    res = "[success] ";
  } else if (ticket_info.status_ == 0) {
    res = "[pending] ";
  } else {
    res = "[refunded] ";
  }
  res += std::string{ticket_info.train_id_} + " ";
  res += std::string{ticket_info.start_} + " ";
  res += date_to_string(ticket_info.date_ + ticket_info.start_time_ / TIME_MAX_IN_DAY) + " ";
  res += time_to_string(ticket_info.start_time_) + " -> ";
  res += std::string{ticket_info.dest_} + " ";
  res += date_to_string(ticket_info.date_ + ticket_info.dest_time_ / TIME_MAX_IN_DAY) + " ";
  res += time_to_string(ticket_info.dest_time_) + " ";
  res += std::to_string(ticket_info.price_) + " " + std::to_string(ticket_info.quantity_);
  return res;
}

auto to_string(const TicketWaitInfo &ticket_wait_info) -> std::string {
  return ticket_wait_info.ticket_id_.ToString();
}

auto get_station_index(const TrainInfo &train_info, const StationID &station_id) -> int {
  for (int i = 0; i < train_info.station_num_; i++) {
    if (train_info.stations_id_[i] == station_id) {
      return i;
    }
  }
  return -1;
}
}  // namespace conless