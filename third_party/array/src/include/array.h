#include "file/file_manager.h"

namespace conless {

template <class T>
class Array {
 public:
  explicit Array(const std::string &file_name, bool inherit_file = true);
  ~Array();

  void Insert(const T &value);  // NOLINT
  auto ValueAt(int index) -> T;       // NOLINT
  void SetValueAt(int index, const T &value);

 private:
  FileManager *file_manager_;
  int size_;
};

}  // namespace conless