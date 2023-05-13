#include "ticket_system.h"

using conless::TicketSystem;

auto main() -> int {
  std::ios::sync_with_stdio(false);
  TicketSystem root("ticket_system", false);
  std::string input;
  while (getline(std::cin, input)) {
    if (input == "exit") {
      break;
    }
    root.AcceptMsg(input);
  }
  return 0;
}