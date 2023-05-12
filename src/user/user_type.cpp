#include "user/user_type.h"
#include <string>

namespace conless {
UserInfo::operator std::string() {
  return std::string{user_name_} + " " + std::string{user_nickname_} + " " + std::string{email_} + " " + std::to_string(privilege_);
}
}  // namespace conless