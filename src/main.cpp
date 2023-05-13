#include "ticket_system.h"

using namespace conless;  // NOLINT

const std::string FILE_NAME = "ticket_system";

auto main() -> int {
  std::ios::sync_with_stdio(false);
  auto *root = new TicketSystem(FILE_NAME, true);
  std::string input;
  while (getline(std::cin, input)) {
    Parser input_msg(input);
    if (input_msg.instruction_ == "exit") {
      std::cout << "[" << input_msg.timestamp_ << "] bye\n";
      break;
    }
    if (input_msg.instruction_ == "clean") {
      delete root;
      root = new TicketSystem(FILE_NAME, false);
      std::cout << "[" << input_msg.timestamp_ << "] 0\n";
      continue;
    }
    root->AcceptMsg(input_msg);
  }
  return 0;
}