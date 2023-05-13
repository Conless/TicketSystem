#include "train/train_type.h"

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
  if (date > 31) {
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
  int hour = time / 60;
  int minute = time % 60;
  std::string res;
  if (hour >= 10) {
    res += std::to_string(hour);
  } else {
    res += "0" + std::string{static_cast<char>('0' + hour)};
  }
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
  std::string res = std::string{train_info.train_id_} + " " + train_info.type_ + "\n";
  for (int i = 0; i < train_info.station_num_; i++) {
    std::string arr_time;
    if (i == 0) {
      arr_time = "xx-xx xx:xx";
    } else {
      int arr_min = train_info.start_time_ + train_info.arr_time_[i];
      int arr_date = train_date_info.date_ + arr_min / TIME_MAX_IN_DAY;
      arr_time = date_to_string(arr_date) + " " + time_to_string(arr_min % TIME_MAX_IN_DAY);
    }
    std::string dep_time;
    if (i == train_info.station_num_ - 1) {
      dep_time = "xx-xx xx:xx";
    } else {
      int dep_min = train_info.start_time_ + train_info.dep_time_[i];
      int dep_date = train_date_info.date_ + dep_min / TIME_MAX_IN_DAY;
      arr_time = date_to_string(dep_date) + " " + time_to_string(dep_min % TIME_MAX_IN_DAY);
    }
    res += std::string{train_info.station_id_[i]} + " ";
    res += arr_time + " -> ";
    res += dep_time + " ";
    if (i != train_info.station_num_ - 1) {
      res += std::to_string(train_date_info.remain_ticket_[i]) + "\n";
    } else {
      res += "x";
    }
    res += "\n";
  }
  return res;
}
}  // namespace conless