#include "user/user_system.h"

namespace conless {

UserSystem::UserSystem(const std::string &file_name, bool inherit_file) : user_info_db_(file_name + "_user_info") {}

void UserSystem::Init(const UserName &username, const UserPassword &passwd, const UserNickname &nickname,
                      const UserEmail &mail_addr) {
  UserInfo new_user_info{username, nickname, passwd, mail_addr, 10};
  user_info_db_.Insert(username, new_user_info);
}

auto UserSystem::AddUser(const UserName &cur_username, const UserName &username, const UserPassword &passwd,
                         const UserNickname &nickname, const UserEmail &mail_addr, int priv) -> bool {
  UserInfo cur_user_info;
  if (!CheckLogin(cur_username, cur_user_info) || cur_user_info.privilege_ <= priv) {
    return false;
  }
  if (user_info_db_.Find(username).first) {
    return false;
  }
  UserInfo new_user_info{username, nickname, passwd, mail_addr, priv};
  user_info_db_.Insert(username, new_user_info);
  return true;
}

auto UserSystem::Login(const UserName &username, const UserPassword &passwd) -> bool {
  auto user_info_iter = user_info_db_.GetIterator(username);
  if (user_info_iter.IsEnd() || user_info_iter->second.login_status_) {
    return false;
  }
  auto &user_info = user_info_iter->second;
  if (user_info.password_ != passwd) {
    return false;
  }
  user_info.login_status_ = true;
  return true;
}

auto UserSystem::Logout(const UserName &username) -> bool {
  auto user_info_iter = user_info_db_.GetIterator(username);
  if (user_info_iter.IsEnd() || !user_info_iter->second.login_status_) {
    return false;
  }
  auto &user_info = user_info_iter->second;
  user_info.login_status_ = false;
  return true;
}

void UserSystem::LogoutAll() {
  for (auto it = user_info_db_.GetBeginIterator(); it != user_info_db_.GetEndIterator(); ++it) {
    it->second.login_status_ = false;
  }
}

auto UserSystem::QueryProfile(const UserName &cur_username, const UserName &username) -> std::string {
  UserInfo cur_user_info;
  if (!CheckLogin(cur_username, cur_user_info)) {
    return "-1";
  }
  auto target_user_info = user_info_db_.Find(username);
  if (!target_user_info.first || (cur_user_info.privilege_ <= target_user_info.second.privilege_ && cur_username != username)) {
    return "-1";
  }
  return std::string{target_user_info.second};
}

auto UserSystem::ModifyProfile(const UserName &cur_username, const UserName &username, const UserPassword &passwd,
                               const UserNickname &nickname, const UserEmail &mail_addr, int priv)
    -> std::string {
  UserInfo cur_user_info;
  if (!CheckLogin(cur_username, cur_user_info) || cur_user_info.privilege_ <= priv) {
    return "-1";
  }
  auto target_user_iter = user_info_db_.GetIterator(username);
  if (target_user_iter.IsEnd()) {
    return "-1";
  }
  auto &user_info = target_user_iter->second;
  if (cur_user_info.privilege_ <= user_info.privilege_ && cur_username != username) {
    return "-1";
  }
  if (passwd != "") {
    user_info.password_ = passwd;
  }
  if (nickname != "") {
    user_info.user_nickname_ = nickname;
  }
  if (mail_addr != "") {
    user_info.email_ = mail_addr;
  }
  if (priv != -1) {
    user_info.privilege_ = priv;
  }
  return std::string{user_info};
}

auto UserSystem::Initialized() const -> bool {
  return !user_info_db_.Empty();
}

auto UserSystem::CheckLogin(const UserName &username) -> bool {
  auto user_find_res = user_info_db_.Find(username);
  if (!user_find_res.first) {
    return false;
  }
  return user_find_res.second.login_status_;
}

auto UserSystem::CheckLogin(const UserName &username, UserInfo &dest) -> bool {
  auto user_find_res = user_info_db_.Find(username);
  if (!user_find_res.first) {
    return false;
  }
  dest = user_find_res.second;
  return dest.login_status_;
}

}  // namespace conless