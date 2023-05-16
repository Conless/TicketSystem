#ifndef TRAIN_SYSTEM_H
#define TRAIN_SYSTEM_H

#include "container/array.h"
#include "storage/index/b_plus_tree_index.h"
#include "storage/index/b_plus_tree_index_nts.h"
#include "train/train_type.h"

namespace conless {

using bustub::BPlusTreeIndex;

class TrainSystem {
 public:
  explicit TrainSystem(const std::string &file_name, bool inherit_file = true);
  ~TrainSystem() = default;

 public:
  auto AddTrain(const TrainID &train_id, int station_num, int seat_num, const vector<std::string> &stations,
                const vector<std::string> &prices, const std::string &start_time_str,
                const vector<std::string> &travel_times, const vector<std::string> &stopover_times,
                const vector<std::string> &sale_date, char type) -> bool;

  auto DeleteTrain(const TrainID &train_id) -> bool;

  auto ReleaseTrain(const TrainID &train_id) -> bool;

  auto QueryTrain(const TrainID &train_id, const std::string &date) -> std::string;

  auto QueryTicket(const std::string &date_str, const StationID &start, const StationID &dest, int sort_tag) -> std::string;

  auto QueryTransfer(const std::string &date_str, const StationID &start, const StationID &dest, int sort_tag) -> std::string;

  auto BuyTicket(const TicketID &ticket_id, const TrainID &train_id, const std::string &date_str, const StationID &start, const StationID &dest, int quantity, bool wait_tag) -> std::string;

  auto QueryOrder(const UserName &username) -> std::string;

  auto RefundTicket(const UserName &username, int order_num) -> bool;

 protected:
  void GetStartTrainsInfo(const vector<TrainStationInfo> &start_trains_station_info,
                          vector<vector<ArrivalInfo>> &stations_after_start, vector<TrainInfo> &start_trains_info, int date);
  void GetDestTrainsInfo(const vector<TrainStationInfo> &dest_trains_station_info,
                         vector<vector<DepartureInfo>> &stations_before_dest, vector<TrainInfo> &dest_trains_info, int date);
  auto GetEarliestDate(const TrainInfo &train_info, int station_index, int arr_date, int arr_time) -> int;

  auto BuySelectedTicket(const TicketWaitInfo &new_ticket, TrainDateInfo &train_date_info) -> bool;

 private:
  BPlusTreeIndex<TrainID, TrainInfo> train_info_db_;
  BPlusTreeIndex<TrainDateID, TrainDateInfo> train_date_info_db_;
  BPlusTreeIndex<TrainStationID, TrainStationInfo> train_station_info_db_;
  BPlusTreeIndex<TicketID, TicketInfo> ticket_info_db_;
  BPlusTreeIndex<TrainDateTicketInfo, TicketWaitInfo> ticket_waitlist_info_db_;
};

}  // namespace conless

#endif