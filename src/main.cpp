#include "ticket_system.h"

using namespace conless; // NOLINT

auto main() -> int {
  std::ios::sync_with_stdio(false);
  TicketSystem root("ticket_system", true);
  std::string input;
  while (getline(std::cin, input)) {
    Parser input_msg(input);
    if (input_msg.instruction_ == "exit") {
      std::cout << "[" << input_msg.timestamp_ << "] bye\n";
      break;
    }
    root.AcceptMsg(input_msg);
  }
  return 0;
}