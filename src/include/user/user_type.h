#ifndef USER_TYPE_H
#define USER_TYPE_H

#include "container/types.h"

namespace conless {

using UserName = StringKey<20>;
using UserNickname = StringKey<20>;
using UserPassword = StringKey<30>;
using UserEmail = StringKey<30>;

struct UserInfo {
  UserName user_name_;
  UserNickname user_nickname_;
  UserPassword password_;
  UserEmail email_;
  int privilege_;
  bool login_status_{false};

  explicit operator std::string();
};

}  // namespace conless

#endif