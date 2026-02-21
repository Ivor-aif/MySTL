//
// Created by Ivor_Aif on 2026/2/21.
//

#ifndef DEQUEUE_H
#define DEQUEUE_H

#include <cassert>

#include "List.h"

namespace mySTL::structures {

template<typename T, typename Alloc = std::allocator<T>>
class Dequeue {
private:
    containers::List<T, Alloc> list;

public:
    Dequeue() = default;
    Dequeue(const Dequeue&) = default;
    Dequeue(Dequeue&&) noexcept = default;
    Dequeue& operator=(const Dequeue&) = default;
    Dequeue& operator=(Dequeue&&) noexcept = default;
    ~Dequeue() = default;

    [[nodiscard]] bool empty() const noexcept {
        return list.empty();
    }

    [[nodiscard]] size_t getSize() const noexcept {
        return list.length();
    }

    void clear() noexcept {
        list.clear();
    }

    void pushFront(const T& value) {
        list.pushFront(value);
    }

    void pushFront(T&& value) {
        list.pushFront(std::move(value));
    }

    void pushBack(const T& value) {
        list.pushBack(value);
    }

    void pushBack(T&& value) {
        list.pushBack(std::move(value));
    }

    T popFront() {
        assert(!empty());
        T value = std::move(list.front());
        list.popFront();
        return value;
    }

    T popBack() {
        assert(!empty());
        T value = std::move(list.back());
        list.popBack();
        return value;
    }

    T& front() {
        return list.front();
    }

    const T& front() const {
        return list.front();
    }

    T& back() {
        return list.back();
    }

    const T& back() const {
        return list.back();
    }

    void swap(Dequeue& other) noexcept(noexcept(list.swap(other.list))) {
        list.swap(other.list);
    }
};

template<typename T, typename Alloc>
void swap(Dequeue<T, Alloc>& left, Dequeue<T, Alloc>& right) noexcept(noexcept(left.swap(right))) {
    left.swap(right);
}

}

#endif // DEQUEUE_H
