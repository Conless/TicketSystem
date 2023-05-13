#ifndef TOKEN_SCANNER_H
#define TOKEN_SCANNER_H

#include <string>

#include "container/stl/vector.h"

namespace conless {

class Lexer : public vector<std::string> {
 public:
  explicit Lexer(std::string str_in_line, char divide_opt = ' ');
};

class Parser {
 public:
  explicit Parser(const std::string &input);
  auto GetString(char opt) const -> std::string;
  auto GetInt(char opt) const -> int;

 public:
  int timestamp_;
  std::string instruction_;
  vector<std::string> parameters_[26];
};

}  // namespace conless

#endif