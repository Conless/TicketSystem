#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fstream>
#include <string>

namespace conless {

constexpr int DEFAULT_PAGE_SIZE = 4096;

class FileManager {
 public:
  explicit FileManager(const std::string &file_name, int page_size = DEFAULT_PAGE_SIZE)
      : file_name_(file_name), page_size_(page_size) {
    db_io_.open(file_name + ".db", std::ios::binary | std::ios::in | std::ios::out);
    log_io_.open(file_name + ".log", std::ios::binary | std::ios::in | std::ios::out);
    if (!db_io_.good()) {
      db_io_.open(file_name + ".db", std::ios::binary | std::ios::trunc | std::ios::out | std::ios::in);
      log_io_.open(file_name + ".log", std::ios::binary | std::ios::trunc | std::ios::out | std::ios::in);
      WriteLog(0);
    }
  }

  ~FileManager() {
    db_io_.close();
    log_io_.close();
  }

  void WritePage(const char *data, int offset) {
    db_io_.seekp(offset * page_size_);
    db_io_.write(data, page_size_);
  }

  void WriteLog(const int &data) {
    log_io_.seekp(0);
    log_io_.write(reinterpret_cast<const char *>(&data), page_size_);
  }

  void ReadPage(char *data, int offset) {
    db_io_.seekg(offset * page_size_);
    db_io_.read(data, page_size_);
  }

  void ReadLog(int &data) {
    log_io_.seekg(0);
    log_io_.read(reinterpret_cast<char *>(&data), sizeof(int));
  }

 private:
  const std::string file_name_;
  const int page_size_;
  std::fstream db_io_;
  std::fstream log_io_;
};

}  // namespace conless

#endif