#ifndef UTILITY_H
#define UTILITY_H

#include <string>

namespace conless {

auto TimeStamp(int timestamp) -> std::string { return "[" + std::to_string(timestamp) + "] "; }

}

#endif