#include "utils/token_scanner.h"

#include <cctype>
#include "utils/exceptions.h"

namespace conless {

void SimplifySpace(std::string &str) {
  while (str[0] == ' ') {
    str.erase(0, 1);
  }
  while (str[str.size() - 1] == ' ') {
    str.erase(str.size() - 1, 1);
  }
  for (int i = 1; i < str.size(); i++) {
    if (str[i] == str[i - 1] && str[i] == ' ') {
      str.erase(i, 1);
      i--;
    }
  }
}

Lexer::Lexer(std::string str_in_line, char divide_opt) {
  SimplifySpace(str_in_line);
  int las = 0;
  int siz = str_in_line.size();
  for (int i = 0; i < siz; i++) {
    if (str_in_line[i] == divide_opt) {
      push_back(str_in_line.substr(las, i - las));
      las = i + 1;
    } else if (i == siz - 1) {
      push_back(str_in_line.substr(las, i - las + 1));
    }
  }
}

auto ReadTimestamp(const std::string &str) -> int {
  int siz = str.size();
  if (siz < 3 || str[0] != '[' || str[siz - 1] != ']') {
    throw Exception("Cannot read timestamp.");
  }
  std::string num_part = str.substr(1, siz - 2);
  return std::stoi(num_part);
}

Parser::Parser(const std::string &input) {
  Lexer input_str(input, ' ');
  if (input_str.size() < 2) {
    throw Exception("Invaild input.");
  }
  timestamp_ = ReadTimestamp(input_str[0]);
  instruction_ = input_str[1];
  for (int i = 2; i < input_str.size(); i += 2) {
    if (input_str[i].size() != 2 || input_str[i][0] != '-' || std::islower(input_str[i][1]) == 0) {
      throw Exception("Input invalid parameters.");
    }
    parameters_[input_str[i][1] - 'a'] = Lexer(input_str[i + 1], '|');
  }
  for (auto & parameter : parameters_) {
    if (parameter.empty()) {
      parameter.push_back("");
    }
  }
}

auto Parser::GetString(char opt) -> std::string {
  if (parameters_[opt - 'a'].empty()) {
    return "";
  }
  return parameters_[opt - 'a'].front();
}

auto Parser::GetInt(char opt) -> int {
  if (parameters_[opt - 'a'].empty()) {
    return -1;
  }
  return std::stoi(parameters_[opt - 'a'].front());
}


}  // namespace conless