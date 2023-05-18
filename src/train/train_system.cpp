#include "train/train_system.h"
#include <algorithm>
#include <climits>
#include <string>
#include "container/types.h"
#include "train/train_type.h"
#include "utils/algorithm.h"
#include "utils/exceptions.h"

namespace conless {

TrainSystem::TrainSystem(const std::string &file_name, bool inherit_file)
    : train_info_db_(file_name + "_train_info"),
      train_date_info_db_(file_name + "_train_date_info"),
      train_station_info_db_(file_name + "_train_station_info", 20),
      ticket_info_db_(file_name + "_ticket_info", 50),
      ticket_waitlist_info_db_(file_name + "_ticket_waitlist_info") {}

auto TrainSystem::AddTrain(const TrainID &train_id, int station_num, int seat_num, const vector<std::string> &stations,
                           const vector<std::string> &prices, const std::string &start_time_str,
                           const vector<std::string> &travel_times, const vector<std::string> &stopover_times,
                           const vector<std::string> &sale_date, char type) -> bool {
  if (train_info_db_.Find(train_id).first) {
    return false;
  }
  TrainInfo train_info{train_id, seat_num, station_num, date_to_int(sale_date.front()), date_to_int(sale_date.back()),
                       type,     false};
  int start_time = time_to_int(start_time_str);

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
      train_info.dep_times_[i] = start_time;
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
    int date_offset = train_info.dep_times_[i] / TIME_MAX_IN_DAY;
    train_station_info_db_.Insert({train_info.stations_id_[i], train_id},
                                  {train_id, train_info.stations_id_[i], i, train_info.start_date_ + date_offset,
                                   train_info.end_date_ + date_offset, train_info.arr_times_[i], train_info.dep_times_[i], train_info.prices_[i]});
  }
  return true;
}

auto TrainSystem::QueryTrain(const TrainID &train_id, const std::string &date) -> std::string {
  auto train_find_res = train_info_db_.Find(train_id);
  if (!train_find_res.first) {
    return "-1";
  }
  const auto &train_info = train_find_res.second;
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

      if (start_index < dest_index && date >= start_train_station_info.dep_date_start_ &&
          date <= start_train_station_info.dep_date_end_) {  // If the train come to dest after start and date is valid
        int start_date = date - start_train_station_info.dep_time_ / TIME_MAX_IN_DAY;
        auto train_date_info = train_date_info_db_.Find({start_train_station_info.train_id_, start_date}).second;

        std::string train_ticket_info = to_string(train_date_info, start_train_station_info, dest_train_station_info, start_date);

        if (sort_tag == 0) {  // Sort by time
          int time = dest_train_station_info.arr_time_ - start_train_station_info.dep_time_;
          trains_ticket_info.push_back({{time, start_train_station_info.train_id_}, train_ticket_info});
        } else {  // Sort by cost
          int cost = dest_train_station_info.price_ - start_train_station_info.price_;
          trains_ticket_info.push_back({{cost, start_train_station_info.train_id_}, train_ticket_info});
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
  std::string result = std::to_string(siz);
  for (int i = 0; i < siz; i++) {
    result += "\n" + trains_ticket_info[i].second;
  }
  return result;
}

void TrainSystem::GetStartTrainsInfo(const vector<TrainStationInfo> &start_trains_station_info,
                                     vector<vector<ArrivalInfo>> &stations_after_start,
                                     vector<TrainInfo> &start_trains_info, int date) {
  start_trains_info.reserve(start_trains_station_info.size());
  for (int i = 0; i < start_trains_station_info.size(); i++) {
    const auto &start_train_station_info = start_trains_station_info[i];
    stations_after_start.push_back({});
    if (date < start_train_station_info.dep_date_start_ || date > start_train_station_info.dep_date_end_) {
      start_trains_info.push_back({});
      continue;
    }
    start_trains_info.push_back(train_info_db_.Find(start_train_station_info.train_id_).second);
    const auto &start_train_info = start_trains_info[i];
    int start_index = start_train_station_info.index_in_train_;

    int start_date = date - start_train_info.dep_times_[start_index] / TIME_MAX_IN_DAY;
    stations_after_start[i].reserve(start_train_info.station_num_ - start_index);
    for (int j = start_index + 1; j < start_train_info.station_num_; j++) {
      int arr_time = start_train_info.arr_times_[j];
      stations_after_start[i].push_back(
          {start_train_info.stations_id_[j], j, start_date + arr_time / TIME_MAX_IN_DAY, arr_time % TIME_MAX_IN_DAY});
    }
    sort(stations_after_start[i].begin(), stations_after_start[i].end(),
         [](const auto &a, const auto &b) { return a.station_id_ < b.station_id_; });
  }
}

void TrainSystem::GetDestTrainsInfo(const vector<TrainStationInfo> &dest_trains_station_info,
                                    vector<vector<DepartureInfo>> &stations_before_dest,
                                    vector<TrainInfo> &dest_trains_info, int date) {
  dest_trains_info.reserve(dest_trains_station_info.size());
  for (int i = 0; i < dest_trains_station_info.size(); i++) {
    const auto &dest_train_station_info = dest_trains_station_info[i];
    dest_trains_info.push_back(train_info_db_.Find(dest_train_station_info.train_id_).second);
    stations_before_dest.push_back(vector<DepartureInfo>());
    const auto &dest_train_info = dest_trains_info[i];
    int dest_index = dest_train_station_info.index_in_train_;

    stations_before_dest[i].reserve(dest_index);
    for (int j = 0; j < dest_index; j++) {
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
  int dep_date_offset = train_info.dep_times_[station_index] / TIME_MAX_IN_DAY;
  int dep_time = train_info.dep_times_[station_index] % TIME_MAX_IN_DAY;
  if (arr_date > train_info.end_date_ + dep_date_offset ||
      (arr_date == train_info.end_date_ + dep_date_offset && arr_time > dep_time)) {
    return -1;
  }
  if (arr_date < train_info.start_date_ + dep_date_offset) {
    return train_info.start_date_;
  }
  if (dep_time < arr_time) {
    return arr_date - dep_date_offset + 1;
  }
  return arr_date - dep_date_offset;
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
  vector<vector<ArrivalInfo>> stations_after_start;
  stations_after_start.reserve(start_trains_station_info.size());
  vector<TrainInfo> start_trains_info;
  GetStartTrainsInfo(start_trains_station_info, stations_after_start, start_trains_info, date);

  vector<vector<DepartureInfo>> stations_before_dest;
  stations_before_dest.reserve(dest_trains_station_info.size());
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

      if (dest_train_info.train_id_ == start_train_info.train_id_) {
        continue;
      }

      for (int k = 0, l = 0; k < stations_after_start[i].size() && l < stations_before_dest[j].size();) {
        const auto &start_arr_info = stations_after_start[i][k];
        const auto &dest_dep_info = stations_before_dest[j][l];

        if (start_arr_info.station_id_ == dest_dep_info.station_id_) {
          int dest_dep_date = GetEarliestDate(dest_train_info, dest_dep_info.index_in_train_,
                                              start_arr_info.arrival_date_, start_arr_info.arrival_time_);
          if (dest_dep_date == -1) {
            k++;
            l++;
            continue;
          }

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
                          dest_dep_date, dest_dep_info.index_in_train_, dest_train_station_info.index_in_train_);
            result.first = (sort_tag == 0) ? ResultKey{{tot_time, tot_cost}, {first_train_id, second_train_id}}
                                           : ResultKey{{tot_cost, tot_time}, {first_train_id, second_train_id}};
          }
          k++;
          l++;
          continue;
        }
        if (start_arr_info.station_id_ < dest_dep_info.station_id_) {
          k++;
        } else {
          l++;
        }
      }
    }
  }
  return result.second;
}

auto TrainSystem::BuyTicket(const TicketID &ticket_id, const TrainID &train_id, const std::string &date,
                            const StationID &start, const StationID &dest, int quantity, bool wait_tag) -> std::string {
  auto train_find_res = train_info_db_.Find(train_id);
  if (!train_find_res.first) {
    return "-1";
  }
  const auto &train_info = train_find_res.second;
  if (train_info.seat_num_ < quantity) {
    return "-1";
  }
  TicketInfo new_ticket{ticket_id, 0, train_id, start, -1, -1, dest, -1, -1, -1, quantity, -1};
  new_ticket.start_index_ = get_station_index(train_info, start);
  new_ticket.dest_index_ = get_station_index(train_info, dest);
  if (new_ticket.start_index_ == -1 || new_ticket.dest_index_ == -1 ||
      new_ticket.start_index_ >= new_ticket.dest_index_) {
    return "-1";
  }
  new_ticket.start_time_ = train_info.dep_times_[new_ticket.start_index_];
  new_ticket.dest_time_ = train_info.arr_times_[new_ticket.dest_index_];
  new_ticket.date_ = date_to_int(date) - new_ticket.start_time_ / TIME_MAX_IN_DAY;
  if (!train_info.released_ || new_ticket.date_ < train_info.start_date_ || new_ticket.date_ > train_info.end_date_) {
    return "-1";
  }
  auto train_date_info_iter = train_date_info_db_.GetIterator({new_ticket.train_id_, new_ticket.date_});
  if (train_date_info_iter.IsEnd()) {
    throw Exception("Iterator error.");
  }
  new_ticket.price_ = train_info.prices_[new_ticket.dest_index_] - train_info.prices_[new_ticket.start_index_];
  auto &train_date_info = train_date_info_iter->second;
  TicketWaitInfo new_ticket_wait = {new_ticket.ticket_id_, new_ticket.start_index_, new_ticket.dest_index_,
                                    new_ticket.quantity_};
  bool buy_res = BuySelectedTicket(new_ticket_wait, train_date_info);
  if (!buy_res && !wait_tag) {
    return "-1";
  }
  if (buy_res) {
    new_ticket.status_ = 1;
    ticket_info_db_.Insert(new_ticket.ticket_id_, new_ticket);
    return std::to_string(new_ticket.price_ * new_ticket.quantity_);
  }
  // The remain case is waitlisted
  ticket_info_db_.Insert(new_ticket.ticket_id_, new_ticket);
  ticket_waitlist_info_db_.Insert({{new_ticket.train_id_, new_ticket.date_}, new_ticket_wait.ticket_id_.second_},
                                  new_ticket_wait);
  return "queue";
}

auto TrainSystem::BuySelectedTicket(const TicketWaitInfo &new_ticket, TrainDateInfo &train_date_info) -> bool {
  int min_remain_ticket = new_ticket.quantity_;
  for (int i = new_ticket.start_index_; i < new_ticket.dest_index_; i++) {
    min_remain_ticket = std::min(min_remain_ticket, train_date_info.remain_tickets_[i]);
  }
  if (min_remain_ticket < new_ticket.quantity_) {
    return false;
  }
  for (int i = new_ticket.start_index_; i < new_ticket.dest_index_; i++) {
    train_date_info.remain_tickets_[i] -= new_ticket.quantity_;
  }
  return true;
}

auto TrainSystem::QueryOrder(const UserName &username) -> std::string {
  vector<TicketInfo> ticket_infos;
  ticket_info_db_.Search({username, 0}, &ticket_infos, TicketID::Comparator(CompareFirst));
  std::string query_res = std::to_string(ticket_infos.size());
  for (int i = ticket_infos.size() - 1; i >= 0; i--) {
    query_res += "\n" + to_string(ticket_infos[i]);
  }
  return query_res;
}

auto TrainSystem::RefundTicket(const UserName &username, int order_num) -> bool {
  vector<TicketInfo> ticket_infos;
  ticket_info_db_.Search({username, 0}, &ticket_infos, TicketID::Comparator(CompareFirst));
  if (ticket_infos.size() < order_num) {
    return false;
  }
  auto target_ticket_iter = ticket_info_db_.GetIterator(ticket_infos[ticket_infos.size() - order_num].ticket_id_);
  if (target_ticket_iter.IsEnd()) {
    throw Exception("Iterator error.");
  }
  const auto target_ticket_info = target_ticket_iter->second;
  if (target_ticket_info.status_ == -1) {
    return false;
  }
  target_ticket_iter->second.status_ = -1;
  auto train_date_iter = train_date_info_db_.GetIterator({target_ticket_info.train_id_, target_ticket_info.date_});
  if (train_date_iter.IsEnd()) {
    throw Exception("Iterator error.");
  }
  auto &train_date_info = train_date_iter->second;
  vector<TicketWaitInfo> ticket_waitlist_infos;
  ticket_waitlist_info_db_.Search({{target_ticket_info.train_id_, target_ticket_info.date_}, 0}, &ticket_waitlist_infos,
                                  TrainDateTicketInfo::Comparator(CompareFirst));
  if (target_ticket_info.status_ == 1) {
    for (int i = target_ticket_info.start_index_; i < target_ticket_info.dest_index_; i++) {
      train_date_info.remain_tickets_[i] += target_ticket_info.quantity_;
    }
    for (const auto &ticket_waitlist_info : ticket_waitlist_infos) {
      if (BuySelectedTicket(ticket_waitlist_info, train_date_info)) {
        auto new_ticket_iter = ticket_info_db_.GetIterator(ticket_waitlist_info.ticket_id_);
        if (new_ticket_iter.IsEnd()) {
          throw Exception("Iterator error.");
        }
        new_ticket_iter->second.status_ = 1;
        ticket_waitlist_info_db_.Delete(
            {{target_ticket_info.train_id_, target_ticket_info.date_}, ticket_waitlist_info.ticket_id_.second_});
      }
    }
  } else {
    for (const auto &ticket_waitlist_info : ticket_waitlist_infos) {
      if (ticket_waitlist_info.ticket_id_ == target_ticket_info.ticket_id_) {
        ticket_waitlist_info_db_.Delete(
            {{target_ticket_info.train_id_, target_ticket_info.date_}, ticket_waitlist_info.ticket_id_.second_});
        break;
      }
    }
  }
  return true;
}

}  // namespace conless
