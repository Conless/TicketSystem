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
    file_io_.open(file_name, std::ios::binary | std::ios::in | std::ios::out);
    if (!file_io_.good()) {
      file_io_.open(file_name, std::ios::binary | std::ios::trunc | std::ios::out | std::ios::in);
      int siz = 0;
      WriteLog(reinterpret_cast<const char *>(&siz));
    }
  }

  void WritePage(const char *data, int offset) {
    file_io_.seekp((offset + 1) * page_size_);
    file_io_.write(data, page_size_);
  }

  void WriteLog(const char *data) {
    file_io_.seekp(0);
    file_io_.write(data, page_size_);
  }

  void ReadPage(char *data, int offset) {
    file_io_.seekg((offset + 1) * page_size_);
    file_io_.read(data, page_size_);
  }

  void ReadLog(char *data) {
    file_io_.seekg(0);
    file_io_.read(data, page_size_);
  }

 private:
  const std::string file_name_;
  const int page_size_;
  std::fstream file_io_;
};

}  // namespace conless

#endif