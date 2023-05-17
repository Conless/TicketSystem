#include "ticket_system.h"

#include <filesystem>
#include <string>

#include "train/train_type.h"
#include "utils/exceptions.h"
#include "utils/utility.h"

namespace conless {

auto ConstructPrepare(const std::string &dir_name, bool inherit_file) -> bool {
  bool res = true;
  if (!inherit_file) {
    res &= std::filesystem::remove_all(dir_name);
  }
  if (!std::filesystem::exists(dir_name)) {
    res &= std::filesystem::create_directory(dir_name);
  }
  return res;
}

TicketSystem::TicketSystem(const std::string &file_name, bool inherit_file)
    : initialized_(ConstructPrepare("data", inherit_file)),
      user_sys_("data/" + file_name, inherit_file),
      train_sys_("data/" + file_name, inherit_file) {
  if (!initialized_) {
    throw Exception("Initialize failed.");
  }
}

void TicketSystem::AcceptMsg(const Parser &input_msg) {
  if (input_msg.instruction_ == "add_user") {
    AddUser(input_msg.timestamp_, input_msg.GetString('c'), input_msg.GetString('u'), input_msg.GetString('p'),
            input_msg.GetString('n'), input_msg.GetString('m'), input_msg.GetInt('g'));
    return;
  }
  if (input_msg.instruction_ == "login") {
    Login(input_msg.timestamp_, input_msg.GetString('u'), input_msg.GetString('p'));
    return;
  }
  if (input_msg.instruction_ == "logout") {
    Logout(input_msg.timestamp_, input_msg.GetString('u'));
    return;
  }
  if (input_msg.instruction_ == "query_profile") {
    QueryProfile(input_msg.timestamp_, input_msg.GetString('c'), input_msg.GetString('u'));
    return;
  }
  if (input_msg.instruction_ == "modify_profile") {
    ModifyProfile(input_msg.timestamp_, input_msg.GetString('c'), input_msg.GetString('u'), input_msg.GetString('p'),
                  input_msg.GetString('n'), input_msg.GetString('m'), input_msg.GetInt('g'));
    return;
  }
  if (input_msg.instruction_ == "add_train") {
    AddTrain(input_msg.timestamp_, input_msg.GetString('i'), input_msg.GetInt('n'), input_msg.GetInt('m'),
             input_msg.GetVector('s'), input_msg.GetVector('p'), input_msg.GetString('x'), input_msg.GetVector('t'),
             input_msg.GetVector('o'), input_msg.GetVector('d'), input_msg.GetChar('y'));
    return;
  }
  if (input_msg.instruction_ == "delete_train") {
    DeleteTrain(input_msg.timestamp_, input_msg.GetString('i'));
    return;
  }
  if (input_msg.instruction_ == "release_train") {
    ReleaseTrain(input_msg.timestamp_, input_msg.GetString('i'));
    return;
  }
  if (input_msg.instruction_ == "query_train") {
    QueryTrain(input_msg.timestamp_, input_msg.GetString('i'), input_msg.GetString('d'));
    return;
  }
  if (input_msg.instruction_ == "query_ticket") {
    QueryTicket(input_msg.timestamp_, input_msg.GetString('s'), input_msg.GetString('t'), input_msg.GetString('d'),
                input_msg.GetString('p'));
    return;
  }
  if (input_msg.instruction_ == "query_transfer") {
    QueryTransfer(input_msg.timestamp_, input_msg.GetString('s'), input_msg.GetString('t'), input_msg.GetString('d'),
                  input_msg.GetString('p'));
    return;
  }
  if (input_msg.instruction_ == "buy_ticket") {
    BuyTicket(input_msg.timestamp_, input_msg.GetString('u'), input_msg.GetString('i'), input_msg.GetString('d'),
              input_msg.GetString('f'), input_msg.GetString('t'), input_msg.GetInt('n'), input_msg.GetString('q'));
    return;
  }
  if (input_msg.instruction_ == "query_order") {
    QueryOrder(input_msg.timestamp_, input_msg.GetString('u'));
    return;
  }
  if (input_msg.instruction_ == "refund_ticket") {
    RefundTicket(input_msg.timestamp_, input_msg.GetString('u'), input_msg.GetInt('n'));
    return;
  }
}

void TicketSystem::Exit() {
  user_sys_.LogoutAll();
}

void TicketSystem::AddUser(int timestamp, const std::string &cur_username, const std::string &username,
                           const std::string &passwd, const std::string &name, const std::string &mail_addr, int priv) {
  if (!user_sys_.Initialized()) {
    user_sys_.Init(UserName(username), UserPassword(passwd), UserNickname(name), UserEmail(mail_addr));
    std::cout << TimeStamp(timestamp) << "0\n";
    return;
  }
  bool add_res = user_sys_.AddUser(UserName(cur_username), UserName(username), UserPassword(passwd), UserNickname(name),
                                   UserEmail(mail_addr), priv);
  if (!add_res) {
    std::cout << TimeStamp(timestamp) << "-1\n";
  } else {
    std::cout << TimeStamp(timestamp) << "0\n";
  }
}

void TicketSystem::Login(int timestamp, const std::string &username, const std::string &passwd) {
  bool login_res = user_sys_.Login(UserName(username), UserPassword(passwd));
  if (!login_res) {
    std::cout << TimeStamp(timestamp) << "-1\n";
  } else {
    std::cout << TimeStamp(timestamp) << "0\n";
  }
}

void TicketSystem::Logout(int timestamp, const std::string &username) {
  bool logout_res = user_sys_.Logout(UserName(username));
  if (!logout_res) {
    std::cout << TimeStamp(timestamp) << "-1\n";
  } else {
    std::cout << TimeStamp(timestamp) << "0\n";
  }
}

void TicketSystem::QueryProfile(int timestamp, const std::string &cur_username, const std::string &username) {
  std::cout << TimeStamp(timestamp) << user_sys_.QueryProfile(UserName(cur_username), UserName(username)) << '\n';
}

void TicketSystem::ModifyProfile(int timestamp, const std::string &cur_username, const std::string &username,
                                 const std::string &passwd, const std::string &name, const std::string &mail_addr,
                                 int priv) {
  std::cout << TimeStamp(timestamp)
            << user_sys_.ModifyProfile(UserName(cur_username), UserName(username), UserPassword(passwd),
                                       UserNickname(name), UserEmail(mail_addr), priv)
            << '\n';
}

void TicketSystem::AddTrain(int timestamp, const std::string &train_id, int station_num, int seat_num,
                            const vector<std::string> &stations, const vector<std::string> &prices,
                            const std::string &start_time, const vector<std::string> &travel_times,
                            const vector<std::string> &stopover_times, const vector<std::string> &sale_date,
                            char type) {
  bool add_res = train_sys_.AddTrain(TrainID(train_id), station_num, seat_num, stations, prices, start_time,
                                     travel_times, stopover_times, sale_date, type);
  if (!add_res) {
    std::cout << TimeStamp(timestamp) << "-1\n";
  } else {
    std::cout << TimeStamp(timestamp) << "0\n";
  }
}

void TicketSystem::DeleteTrain(int timestamp, const std::string &train_id) {
  bool delete_res = train_sys_.DeleteTrain(TrainID(train_id));
  if (!delete_res) {
    std::cout << TimeStamp(timestamp) << "-1\n";
  } else {
    std::cout << TimeStamp(timestamp) << "0\n";
  }
}

void TicketSystem::ReleaseTrain(int timestamp, const std::string &train_id) {
  bool release_res = train_sys_.ReleaseTrain(TrainID(train_id));
  if (!release_res) {
    std::cout << TimeStamp(timestamp) << "-1\n";
  } else {
    std::cout << TimeStamp(timestamp) << "0\n";
  }
}

void TicketSystem::QueryTrain(int timestamp, const std::string &train_id, const std::string &date) {
  std::cout << TimeStamp(timestamp) << train_sys_.QueryTrain(TrainID(train_id), date) << '\n';
}

void TicketSystem::QueryTicket(int timestamp, const std::string &start, const std::string &dest,
                               const std::string &date, const std::string &sort_tag) {
  std::cout << TimeStamp(timestamp)
            << train_sys_.QueryTicket(date, StationID(start), StationID(dest), sort_tag == "cost" ? 1 : 0) << '\n';
}

void TicketSystem::QueryTransfer(int timestamp, const std::string &start, const std::string &dest,
                                 const std::string &date, const std::string &sort_tag) {
  std::cout << TimeStamp(timestamp)
            << train_sys_.QueryTransfer(date, StationID(start), StationID(dest), sort_tag == "cost" ? 1 : 0) << '\n';
}

void TicketSystem::BuyTicket(int timestamp, const std::string &user_name, const std::string &train_id,
                             const std::string &date, const std::string &start, const std::string &dest, int quantity,
                             const std::string &wait_tag) {
  if (!user_sys_.CheckLogin(UserName(user_name))) {
    std::cout << TimeStamp(timestamp) << "-1\n";
    return;
  }
  TicketID new_ticket_id{UserName(user_name), timestamp};
  std::string buy_res = train_sys_.BuyTicket(new_ticket_id, TrainID(train_id), date, StationID(start), StationID(dest),
                                             quantity, wait_tag == "true");
  std::cout << TimeStamp(timestamp) << buy_res << '\n';
}

void TicketSystem::QueryOrder(int timestamp, const std::string &user_name) {
  if (!user_sys_.CheckLogin(UserName(user_name))) {
    std::cout << TimeStamp(timestamp) << "-1\n";
    return;
  }
  std::cout << TimeStamp(timestamp) << train_sys_.QueryOrder(UserName(user_name)) << '\n';
}

void TicketSystem::RefundTicket(int timestamp, const std::string &user_name, int order_num) {
  if (!user_sys_.CheckLogin(UserName(user_name))) {
    std::cout << TimeStamp(timestamp) << "-1\n";
    return;
  }
  bool refund_res = train_sys_.RefundTicket(UserName(user_name), order_num == -1 ? 1 : order_num);
  if (!refund_res) {
    std::cout << TimeStamp(timestamp) << "-1\n";
  } else {
    std::cout << TimeStamp(timestamp) << "0\n";
  }
}

}  // namespace conless