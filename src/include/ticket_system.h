#ifndef TICKET_SYSTEM_H
#define TICKET_SYSTEM_H

#include "user/user_system.h"

namespace conless {

class TicketSystem {
 public:
  explicit TicketSystem(const std::string &file_name, bool inherit_file = true);
  ~TicketSystem() = default;

  void AcceptMsg(const std::string &input);

 private:
  void AddUser(int timestamp, const std::string &cur_username, const std::string &username, const std::string &passwd,
               const std::string &name, const std::string &mail_addr, int priv);

  void Login(int timestamp, const std::string &username, const std::string &passwd);

  void Logout(int timestamp, const std::string &username);

  void QueryProfile(int timestamp, const std::string &cur_username, const std::string &username);

  void ModifyProfile(int timestamp, const std::string &cur_username, const std::string &username,
                     const std::string &passwd, const std::string &name, const std::string &mail_addr, int priv);

 private:
  const bool initialized_;
  UserSystem user_sys_;
  // TrainSystem train_sys_;
};

}  // namespace conless

#endif