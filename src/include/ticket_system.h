#ifndef TICKET_SYSTEM_H
#define TICKET_SYSTEM_H

#include "train/train_system.h"
#include "user/user_system.h"
#include "utils/token_scanner.h"
#include "utils/utility.h"

namespace conless {

class TicketSystem {
 public:
  explicit TicketSystem(const std::string &file_name, bool inherit_file = true);
  ~TicketSystem() = default;

  void AcceptMsg(const Parser &input_msg);

  //  private:
  void AddUser(int timestamp, const std::string &cur_username, const std::string &username, const std::string &passwd,
               const std::string &name, const std::string &mail_addr, int priv);

  void Login(int timestamp, const std::string &username, const std::string &passwd);

  void Logout(int timestamp, const std::string &username);

  void QueryProfile(int timestamp, const std::string &cur_username, const std::string &username);

  void ModifyProfile(int timestamp, const std::string &cur_username, const std::string &username,
                     const std::string &passwd, const std::string &name, const std::string &mail_addr, int priv);

  void AddTrain(int timestamp, const std::string &train_id, int station_num, int seat_num,
                const vector<std::string> &stations, const vector<std::string> &prices, const std::string &start_time,
                const vector<std::string> &travel_times, const vector<std::string> &stopover_times,
                const vector<std::string> &sale_date, char type);

  void DeleteTrain(int timestamp, const std::string &train_id);

  void ReleaseTrain(int timestamp, const std::string &train_id);

  void QueryTrain(int timestamp, const std::string &train_id, const std::string &date);

  void QueryTicket(int timestamp, const std::string &start, const std::string &dest, const std::string &date,
                   const std::string &sort_tag);

  void QueryTransfer(int timestamp, const std::string &start, const std::string &dest, const std::string &date,
                     const std::string &sort_tag);

  void BuyTicket(int timestamp, const std::string &user_name, const std::string &train_id, const std::string &date,
                 const std::string &start, const std::string &dest, int quantity, const std::string &wait_tag);

  void QueryOrder(int timestamp, const std::string &user_name);

  void RefundTicket(int timestamp, const std::string &user_name, int order_num);
  

 private:
  const bool initialized_;
  UserSystem user_sys_;
  TrainSystem train_sys_;
};

}  // namespace conless

#endif