#include "container/array.h"
#include "train/train_type.h"

namespace conless {

template <class T>
Array<T>::Array(const std::string &file_name, bool inherit_file) {
  file_manager_ = new FileManager(file_name, sizeof(T));
  size_ = 0;
  if (!inherit_file) {
    file_manager_->WriteLog(size_);
  } else {
    file_manager_->ReadLog(size_);
  }
}

template <class T>
Array<T>::~Array() {
  file_manager_->WriteLog(size_);
  delete file_manager_;
}

template <class T>
auto Array<T>::Insert(const T &value) -> int {
  file_manager_->WritePage(reinterpret_cast<const char *>(&value), size_);
  return size_++;
}

template <class T>
auto Array<T>::ValueAt(int index) -> T {
  T data;
  file_manager_->ReadPage(reinterpret_cast<char *>(&data), index);
  return data;
}

template <class T>
void Array<T>::SetValueAt(int index, const T &value) {
  file_manager_->WritePage(reinterpret_cast<const char *>(&value), index);
}

template class Array<int>;
template class Array<TicketUserInfo>;

}  // namespace conless