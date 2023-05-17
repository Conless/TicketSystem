#ifndef USER_SYSTEM_H
#define USER_SYSTEM_H

#include "storage/index/b_plus_tree_index_nts.h"
#include "user/user_type.h"

namespace conless {

using bustub::BPlusTreeIndex;

class UserSystem {
 public:
  explicit UserSystem(const std::string &file_name, bool inherit_file = true);
  ~UserSystem() = default;

 public:
  void Init(const UserName &username, const UserPassword &passwd, const UserNickname &nickname,
            const UserEmail &mail_addr);
  auto AddUser(const UserName &cur_username, const UserName &username, const UserPassword &passwd,
               const UserNickname &nickname, const UserEmail &mail_addr, int priv) -> bool;

  auto Login(const UserName &username, const UserPassword &passwd) -> bool;

  auto Logout(const UserName &username) -> bool;
  
  void LogoutAll();

  auto QueryProfile(const UserName &cur_username, const UserName &username) -> std::string;

  auto ModifyProfile(const UserName &cur_username, const UserName &username, const UserPassword &passwd,
                     const UserNickname &nickname, const UserEmail &mail_addr, int priv) -> std::string;

  auto Initialized() const -> bool;

  auto CheckLogin(const UserName &username) -> bool;

  auto CheckLogin(const UserName &username, UserInfo &dest) -> bool;
 private:
  BPlusTreeIndex<UserName, UserInfo> user_info_db_;
};

}  // namespace conless

#endif