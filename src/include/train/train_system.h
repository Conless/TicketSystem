#ifndef TRAIN_SYSTEM_H
#define TRAIN_SYSTEM_H

#include "storage/index/b_plus_tree_index_nts.h"
#include "train/train_type.h"

namespace conless {

using bustub::BPlusTreeIndex;

class TrainSystem {
 public:
  explicit TrainSystem(const std::string &file_name, bool inherit_file = true);
  ~TrainSystem() = default;

 public:
  auto AddTrain(const TrainID &train_id, int station_num, int seat_num, const vector<StationID> &stations,
                const vector<int> &prices, const vector<int> &dep_time, const vector<int> &arr_time, int start_date,
                int end_date, char type) -> bool;

  auto DeleteTrain(const TrainID &train_id) -> bool;

  auto ReleasedTrain(const TrainID &train_id) -> bool;

  auto QueryTrain(const TrainID &train_id, int date) -> std::string;

  auto QueryTicket(int date, const StationID &start, const StationID &dest, int sort_tag) -> std::string;

  auto QueryTransfer(int date, const StationID &start, const StationID &dest, int sort_tag) -> std::string;

  auto BuyTicket(const TicketInfo &ticket, bool wait_tag) -> std::string;

  auto QueryOrder(const UserName &username) -> std::string;

  auto RefundTicket(const UserName &username, int order_num);

 private:
  BPlusTreeIndex<TrainID, TrainInfo> train_info_db_;
  BPlusTreeIndex<TrainDateID, TrainDateInfo> train_date_info_db_;
  BPlusTreeIndex<TrainStationID, TrainStationInfo> train_station_info_db_;
  BPlusTreeIndex<TicketID, TicketInfo> ticket_info_db_;
};

}  // namespace conless

#endif