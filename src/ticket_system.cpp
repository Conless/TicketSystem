#include "ticket_system.h"

#include <filesystem>

#include "utils/exceptions.h"

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
    : initialized_(ConstructPrepare("data", inherit_file)), user_sys_("data/" + file_name, inherit_file) {
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
}

void TicketSystem::AddUser(int timestamp, const std::string &cur_username, const std::string &username,
                           const std::string &passwd, const std::string &name, const std::string &mail_addr, int priv) {
  if (!user_sys_.Initialized()) {
    user_sys_.Init(UserName(username), UserPassword(passwd), UserNickname(name), UserEmail(mail_addr));
    std::cout << "[" << timestamp << "] "
              << "0\n";
    return;
  }
  bool add_res = user_sys_.AddUser(UserName(cur_username), UserName(username), UserPassword(passwd), UserNickname(name),
                                   UserEmail(mail_addr), priv);
  if (!add_res) {
    std::cout << "[" << timestamp << "] "
              << "-1\n";
  } else {
    std::cout << "[" << timestamp << "] "
              << "0\n";
  }
}

void TicketSystem::Login(int timestamp, const std::string &username, const std::string &passwd) {
  bool login_res = user_sys_.Login(UserName(username), UserPassword(passwd));
  if (!login_res) {
    std::cout << "[" << timestamp << "] "
              << "-1\n";
  } else {
    std::cout << "[" << timestamp << "] "
              << "0\n";
  }
}

void TicketSystem::Logout(int timestamp, const std::string &username) {
  bool logout_res = user_sys_.Logout(UserName(username));
  if (!logout_res) {
    std::cout << "[" << timestamp << "] "
              << "-1\n";
  } else {
    std::cout << "[" << timestamp << "] "
              << "0\n";
  }
}

void TicketSystem::QueryProfile(int timestamp, const std::string &cur_username, const std::string &username) {
  std::cout << "[" << timestamp << "] " << user_sys_.QueryProfile(UserName(cur_username), UserName(username)) << '\n';
}

void TicketSystem::ModifyProfile(int timestamp, const std::string &cur_username, const std::string &username,
                                 const std::string &passwd, const std::string &name, const std::string &mail_addr,
                                 int priv) {
  std::cout << "[" << timestamp << "] " << user_sys_.ModifyProfile(UserName(cur_username), UserName(username), UserPassword(passwd),
                                       UserNickname(name), UserEmail(mail_addr), priv) << '\n';
}

}  // namespace conless