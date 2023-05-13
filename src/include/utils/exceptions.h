#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include <utility>

namespace conless {

class Exception : public std::exception {
 public:
  explicit Exception(std::string what) : what_(std::move(what)) {}
  auto what() -> std::string { return what_; }

 private:
  std::string what_;
};

}  // namespace conless

#endif