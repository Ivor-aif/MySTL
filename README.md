# MySTL

A modern C++20 infrastructure library.

---

## 📖 Introduction

**MySTL** is a comprehensive C++ library implementing various standard template library components and utility data structures. Built with modern **C++20** standards, it serves as both a learning resource and a practical toolkit for C++ developers.

### ✨ Key Features

- **Array**: Dynamic array implementation (equivalent to `std::vector`).
- **List**: Doubly linked list implementation (equivalent to `std::list`).
- **String**: Custom string manipulation class (equivalent to `std::string`).
- **Network**: Networking utilities for connectivity.
- **Hash**: Hash table implementation (equivalent to `std::hash`) with KeyValue pairs.
- **Stack**: LIFO (Last-In, First-Out) data structure.
- **Queue**: FIFO (First-In, First-Out) data structure.
- **Dequeue**: Double-ended queue implementation based on linked list.
- **BiTree**: Binary Tree data structure implementation.
- **Trie**: Dictionary Tree (Prefix Tree) implementation.
- **Heap**: Priority Queue implementation.

---

## 🛠️ Build Configuration

This project is configured using **CMake** (minimum version 3.31). It targets the **C++20** standard and builds the `MySTL` executable with static linking.

For detailed build configuration, please refer to [CMakeLists.txt](CMakeLists.txt).

---

## 📄 License

This project is licensed under the **MIT License**.

For full license details, please refer to the [LICENSE](LICENSE) file.

---

## 📅 Development Log

**2026-01-31 first**: Array (also called `std::vector` for *C++*) develop finished.\
**2026-02-01 first**: The use of `Array` created, Welcome to submit any other usages.\
**2026-02-02 first**: Ready for `List`(linked list).\
**2026-02-05 first**: List (also called `std::list` for *C++*) develop finished.\
**2026-02-08 first**: The use of `List` created, Welcome to submit any other usages. And repaired some known bugs.\
**2026-02-09 first**: Network created and initial development has completed.\
**2026-02-10 first**: The use of `Network` created, Welcome to submit any other usages. And repaired some known bugs.\
**2026-02-12 first**: String (also `std::string`) created and add some basic methods.\
**2026-02-14 first**: The use of `String` created, Welcome to submit any other usages. And repaired some known bugs.\
**2026-02-16 first**: Hash (also `std::hash`) and KeyValue (also `std::pair`) develop finished.\
**2026-02-16 second**: The use of `Hash` created, Welcome to submit any other usages. And repaired some known bugs.\
**2026-02-17 first**: Stack and queue created. Due to the continuous storage, queue is only for light use, too large queue will make `pop()` slowly.\
**2026-02-17 second**: The use of `Stack` and `Queue` created, Welcome to submit any other usages. And repaired some known bugs.\
**2026-02-21 first**: Dequeue(based on linked list) created and developed finish. Welcome to submit any other usages.\
**2026-02-24 first**: BiTree(binary tree) created and developed finish. Welcome to submit any other usages.\
**2026-02-28 first**: Trie(dictionary tree) created and developed finish. Welcome to submit any other usages.\
**2026-02-28 second**: **_The phased development is completed._** Heap(priority queue) created and developed finish. Welcome to submit any other usages.
