#ifndef TICKET_SYSTEM_BPLUS_TREE_H
#define TICKET_SYSTEM_BPLUS_TREE_H

#include <iostream>
#include <fstream>
#include <filesystem>

#include "utils/container/pair.h"
#include "utils/container/vector.h"

namespace conless {

namespace storage {

constexpr int DEGREE = 10;

using NodeIndex = int;

template <class Key, class T> class BPlusTreeInternalNode {
  public:
    int size;
    bool leaf_below;
    utils::pair<Key, T> data[DEGREE << 1];
    NodeIndex ptr[DEGREE << 1];
};

template <class Key, class T> class BPlusTreeLeafNode {
  public:
    int size;
    utils::pair<Key, T> data[DEGREE << 1];
};

/**
 * @brief class BPlusTree
 *
 * @tparam Key
 * @tparam T
 * @tparam Compare
 */
template <class Key, class T> class BPlusTree {
  public:
    using value_type = utils::pair<const Key, T>;

  public:
    BPlusTree(std::string file_name) {
        std::string internal_node_data = "data/" + file_name + "_internal_node.dat";
        std::string leaf_node_data = "data/" + file_name + "_leaf_node.dat";

    }

  private:
    std::fstream file;
    std::string file_name;
};

// template <class Key, class T> class map : public BPlusTree<Key, T> {
//   public:
//     using value_type = typename BPlusTree<Key, T>::value_type;
//   public:
//     /**
//      * @brief Construct a new map object by the default constructor of BPlusTree
//      *
//      */
//     map(std::string file_name) : BPlusTree<Key, T>() {}

//     /**
//      * @brief Construct a new map object from another BPlusTree
//      *
//      * @param other
//      */
//     map(std::string file_name, const map &other) : BPlusTree<Key, T>(other) {}

//     /**
//      * @brief Assignment operator
//      *
//      * @param other
//      * @return map&
//      */
//     map &operator=(const map &other) {
//         BPlusTree<Key, T>::operator=(other);
//         return *this;
//     }

//     /**
//      * @brief Destroy the map object using the same method in BPlusTree
//      *
//      */
//     ~map() {}

//   public:
//     T &at(const Key &key) {}

//     const T &at(const Key &key) const {}

//     T &operator[](const Key &key) {}

//     const T &operator[](const Key &key) const { return at(key); }

//     void insert(const value_type &value) {}
// };

// template class map<std::string, int>;

} // namespace storage

} // namespace conless



#endif