#include "train/train_system.h"
#include <algorithm>
#include <climits>
#include "container/types.h"
#include "train/train_type.h"
#include "utils/algorithm.h"

namespace conless {

TrainSystem::TrainSystem(const std::string &file_name, bool inherit_file)
    : train_info_db_(file_name + "_train_info"),
      train_date_info_db_(file_name + "_train_date_info"),
      train_station_info_db_(file_name + "_train_station_info"),
      ticket_info_db_(file_name + "_ticket_info") {}

auto TrainSystem::AddTrain(const TrainID &train_id, int station_num, int seat_num, const vector<std::string> &stations,
                           const vector<std::string> &prices, const std::string &start_time,
                           const vector<std::string> &travel_times, const vector<std::string> &stopover_times,
                           const vector<std::string> &sale_date, char type) -> bool {
  if (train_info_db_.Find(train_id).first) {
    return false;
  }
  TrainInfo train_info{train_id,
                       seat_num,
                       station_num,
                       time_to_int(start_time),
                       date_to_int(sale_date.front()),
                       date_to_int(sale_date.back()),
                       type};

  for (int i = 0; i < station_num; i++) {
    train_info.stations_id_[i] = StationID{stations[i]};
    if (i > 0) {
      train_info.prices_[i] = train_info.prices_[i - 1] + std::stoi(prices[i - 1]);
      train_info.arr_times_[i] = train_info.dep_times_[i - 1] + std::stoi(travel_times[i - 1]);
      if (i != station_num - 1) {
        train_info.dep_times_[i] = train_info.arr_times_[i] + std::stoi(stopover_times[i - 1]);
      }
    } else {
      train_info.prices_[i] = 0;
      train_info.dep_times_[i] = train_info.start_time_;
    }
  }
  train_info_db_.Insert(train_id, train_info);
  return true;
}

auto TrainSystem::DeleteTrain(const TrainID &train_id) -> bool {
  auto target_train_info = train_info_db_.Find(train_id);
  if (!target_train_info.first || target_train_info.second.released_) {
    return false;
  }
  train_info_db_.Delete(train_id);
  return true;
}

auto TrainSystem::ReleaseTrain(const TrainID &train_id) -> bool {
  auto train_info_iter = train_info_db_.GetIterator(train_id);
  if (train_info_iter.IsEnd() || train_info_iter->second.released_) {
    return false;
  }
  auto &train_info = train_info_iter->second;
  train_info.released_ = true;
  for (int i = train_info.start_date_; i <= train_info.end_date_; i++) {
    TrainDateInfo train_date_info{train_id, i};
    std::fill_n(train_date_info.remain_tickets_, train_info.station_num_ - 1, train_info.seat_num_);
    train_date_info_db_.Insert({train_id, i}, train_date_info);
  }
  for (int i = 0; i < train_info.station_num_; i++) {
    train_station_info_db_.Insert({train_info.stations_id_[i], train_id}, {train_id, train_info.stations_id_[i], i});
  }
  return true;
}

auto TrainSystem::QueryTrain(const TrainID &train_id, const std::string &date) -> std::string {
  auto train_find_res = train_info_db_.Find(train_id);
  if (!train_find_res.first) {
    return "-1";
  }
  const auto &train_info = train_find_res.second;
  if (!train_info.released_) {
    return "-1";
  }
  int date_num = date_to_int(date);
  if (date_num < train_info.start_date_ || date_num > train_info.end_date_) {
    return "-1";
  }
  if (train_info.released_) {
    return to_string(train_info, train_date_info_db_.Find({train_id, date_num}).second);
  }
  return to_string(train_info, date_num);
}

auto TrainSystem::QueryTicket(const std::string &date_str, const StationID &start, const StationID &dest, int sort_tag)
    -> std::string {
  int date = date_to_int(date_str);
  // Find all the trains via the start and destination
  vector<TrainStationInfo> start_trains_station_info;
  train_station_info_db_.Search({start, ""}, &start_trains_station_info, TrainStationID::Comparator(CompareFirst));
  if (start_trains_station_info.empty()) {
    return "0";
  }
  vector<TrainStationInfo> dest_trains_station_info;
  train_station_info_db_.Search({dest, ""}, &dest_trains_station_info, TrainStationID::Comparator(CompareFirst));
  if (dest_trains_station_info.empty()) {
    return "0";
  }

  // Using the method like merge sort
  vector<std::pair<std::pair<int, TrainID>, std::string>> trains_ticket_info;
  for (int i = 0, j = 0; i < start_trains_station_info.size() && j < dest_trains_station_info.size();) {
    const auto &start_train_station_info = start_trains_station_info[i];
    const auto &dest_train_station_info = dest_trains_station_info[j];

    if (start_train_station_info.train_id_ == dest_train_station_info.train_id_) {  // If two trains are the same
      int start_index = start_train_station_info.index_in_train_;
      int dest_index = dest_train_station_info.index_in_train_;

      if (start_index < dest_index) {  // If the train come to dest after start
        auto train_find_res = train_info_db_.Find(start_train_station_info.train_id_);
        const auto &train_info = train_find_res.second;

        int start_date = date - train_info.dep_times_[start_index] / TIME_MAX_IN_DAY;

        if (train_find_res.first && start_date >= train_info.start_date_ &&
            start_date <= train_info.end_date_) {  // Date is valid
          auto train_date_info = train_date_info_db_.Find({train_info.train_id_, start_date}).second;

          std::string train_ticket_info = to_string(train_info, train_date_info, start_date, start_index, dest_index);

          if (sort_tag == 0) {  // Sort by time
            int time = train_info.arr_times_[dest_index] - train_info.dep_times_[start_index];
            trains_ticket_info.push_back({{time, train_info.train_id_}, train_ticket_info});
          } else {  // Sort by cost
            int cost = train_info.prices_[dest_index] - train_info.prices_[start_index];
            trains_ticket_info.push_back({{cost, train_info.train_id_}, train_ticket_info});
          }
        }
      }
      i++;
      j++;
      continue;
    }
    if (start_train_station_info.train_id_ < dest_train_station_info.train_id_) {
      i++;
    } else {
      j++;
    }
  }

  sort(trains_ticket_info.begin(), trains_ticket_info.end());
  int siz = trains_ticket_info.size();
  std::string result = std::to_string(siz) + "\n";
  for (int i = 0; i < siz; i++) {
    result += trains_ticket_info[i].second;
    if (i < siz - 1) {
      result += "\n";
    }
  }
  return result;
}

void TrainSystem::GetStartTrainsInfo(const vector<TrainStationInfo> &start_trains_station_info,
                                     vector<ArrivalInfo> *stations_after_start, vector<TrainInfo> &start_trains_info,
                                     int date) {
  start_trains_info.reserve(start_trains_station_info.size());
  for (int i = 0; i < start_trains_station_info.size(); i++) {
    const auto &start_train_station_info = start_trains_station_info[i];
    start_trains_info.push_back(train_info_db_.Find(start_train_station_info.train_id_).second);
    const auto &start_train_info = start_trains_info[i];
    int start_index = start_train_station_info.index_in_train_;

    int start_date = date - start_train_info.dep_times_[start_index] / TIME_MAX_IN_DAY;
    if (start_date < start_train_info.start_date_ || start_date > start_train_info.end_date_) {
      continue;
    }

    stations_after_start[i].reserve(start_train_info.station_num_ - start_index);
    for (int j = start_index + 1; j < start_train_info.station_num_; j++) {
      int arr_time = start_train_info.arr_times_[j];
      stations_after_start[i].push_back(
          {start_train_info.stations_id_[j], j, date + arr_time / TIME_MAX_IN_DAY, arr_time % TIME_MAX_IN_DAY});
    }
    sort(stations_after_start[i].begin(), stations_after_start[i].end(),
         [](const auto &a, const auto &b) { return a.station_id_ < b.station_id_; });
  }
}

void TrainSystem::GetDestTrainsInfo(const vector<TrainStationInfo> &dest_trains_station_info,
                                    vector<DepartureInfo> *stations_before_dest, vector<TrainInfo> &dest_trains_info,
                                    int date) {
  dest_trains_info.reserve(dest_trains_station_info.size());
  for (int i = 0; i < dest_trains_station_info.size(); i++) {
    const auto &dest_train_station_info = dest_trains_station_info[i];
    dest_trains_info.push_back(train_info_db_.Find(dest_train_station_info.train_id_).second);
    const auto &dest_train_info = dest_trains_info[i];
    stations_before_dest[i].reserve(dest_train_info.station_num_ - dest_train_station_info.index_in_train_);
    for (int j = dest_train_station_info.index_in_train_ + 1; j < dest_train_info.station_num_; j++) {
      stations_before_dest[i].push_back({
          dest_train_info.stations_id_[j],
          j,
      });
    }
    sort(stations_before_dest[i].begin(), stations_before_dest[i].end(),
         [](const auto &a, const auto &b) { return a.station_id_ < b.station_id_; });
  }
}

auto TrainSystem::GetEarliestDate(const TrainInfo &train_info, int station_index, int arr_date, int arr_time) -> int {
  return arr_date;
}

auto TrainSystem::QueryTransfer(const std::string &date_str, const StationID &start, const StationID &dest,
                                int sort_tag) -> std::string {
  int date = date_to_int(date_str);
  vector<TrainStationInfo> start_trains_station_info;
  train_station_info_db_.Search({start, ""}, &start_trains_station_info, TrainStationID::Comparator(CompareFirst));
  if (start_trains_station_info.empty()) {
    return "0";
  }
  vector<TrainStationInfo> dest_trains_station_info;
  train_station_info_db_.Search({dest, ""}, &dest_trains_station_info, TrainStationID::Comparator(CompareFirst));
  if (dest_trains_station_info.empty()) {
    return "0";
  }
  vector<ArrivalInfo> stations_after_start[STATION_NUM_MAX];
  vector<TrainInfo> start_trains_info;
  GetStartTrainsInfo(start_trains_station_info, stations_after_start, start_trains_info, date);

  vector<DepartureInfo> stations_before_dest[STATION_NUM_MAX];
  vector<TrainInfo> dest_trains_info;
  GetDestTrainsInfo(dest_trains_station_info, stations_before_dest, dest_trains_info, date);

  using ResultKey = std::pair<std::pair<int, int>, std::pair<TrainID, TrainID>>;
  std::pair<ResultKey, std::string> result{{{INT_MAX, INT_MAX}, {"", ""}}, "0"};

  for (int i = 0; i < start_trains_info.size(); i++) {
    if (stations_after_start[i].empty()) {
      continue;
    }
    const auto &start_train_info = start_trains_info[i];
    const auto &start_train_station_info = start_trains_station_info[i];

    int start_dep_date = date - start_train_info.dep_times_[start_train_station_info.index_in_train_] / TIME_MAX_IN_DAY;
    int start_date = date;
    int start_time = start_train_info.dep_times_[start_train_station_info.index_in_train_] % TIME_MAX_IN_DAY;

    for (int j = 0; j < dest_trains_info.size(); j++) {
      if (stations_before_dest[j].empty()) {
        continue;
      }
      const auto &dest_train_info = dest_trains_info[j];
      const auto &dest_train_station_info = dest_trains_station_info[j];

      for (int k = 0, l = 0; k < stations_after_start[i].size() && l < stations_before_dest[j].size();) {
        const auto &start_arr_info = stations_after_start[i][k];
        const auto &dest_dep_info = stations_before_dest[j][l];

        if (start_arr_info.station_id_ == dest_dep_info.station_id_) {
          int dest_dep_date = GetEarliestDate(dest_train_info, dest_dep_info.index_in_train_,
                                              start_arr_info.arrival_date_, start_arr_info.arrival_time_);

          int dest_date =
              dest_dep_date + dest_train_info.arr_times_[dest_train_station_info.index_in_train_] / TIME_MAX_IN_DAY;
          int dest_time = dest_train_info.arr_times_[dest_train_station_info.index_in_train_] % TIME_MAX_IN_DAY;

          int start_cost = start_train_info.prices_[start_arr_info.index_in_train_] -
                           start_train_info.prices_[start_train_station_info.index_in_train_];
          int dest_cost = dest_train_info.prices_[dest_train_station_info.index_in_train_] -
                          dest_train_info.prices_[dest_dep_info.index_in_train_];

          int tot_time = dest_time - start_time + (dest_date - start_date) * TIME_MAX_IN_DAY;
          int tot_cost = start_cost + dest_cost;

          auto first_train_id = start_train_info.train_id_;
          auto second_train_id = dest_train_info.train_id_;
          if ((sort_tag == 0 && ResultKey{{tot_time, tot_cost}, {first_train_id, second_train_id}} < result.first) ||
              (sort_tag == 1 && ResultKey{{tot_cost, tot_time}, {first_train_id, second_train_id}} < result.first)) {
            result.second = "";
            result.second +=
                to_string(start_train_info, train_date_info_db_.Find({first_train_id, start_dep_date}).second,
                          start_dep_date, start_train_station_info.index_in_train_, start_arr_info.index_in_train_);
            result.second += "\n";
            result.second +=
                to_string(dest_train_info, train_date_info_db_.Find({second_train_id, dest_dep_date}).second,
                          dest_dep_date, dest_dep_info.index_in_train_, start_arr_info.index_in_train_);
            result.first = (sort_tag == 0) ? ResultKey{{tot_time, tot_cost}, {first_train_id, second_train_id}}
                                           : ResultKey{{tot_cost, tot_time}, {first_train_id, second_train_id}};
          }
        }
      }
    }
  }
  return result.second;
}

}  // namespace conless