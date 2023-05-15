#include "utils/utility.h"

namespace conless {

auto TimeStamp(int timestamp) -> std::string { return "[" + std::to_string(timestamp) + "] "; }

}  // namespace conless
