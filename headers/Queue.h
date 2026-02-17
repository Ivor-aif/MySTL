//
// Created by Ivor_Aif on 2026/2/17.
//

#ifndef QUEUE_H
#define QUEUE_H

#include <cassert>

#include "Containers.h"

namespace mySTL::structures {

template<typename T, typename Alloc = std::allocator<T>>
class Queue {
private:
    containers::detail::conStorage<T, Alloc> data;

public:
    Queue() = default;
    Queue(const Queue&) = default;
    Queue(Queue&&) noexcept(std::is_nothrow_move_constructible_v<Alloc>) = default;
    Queue& operator=(const Queue&) = default;
    Queue& operator=(Queue&&) noexcept(std::is_nothrow_move_assignable_v<Alloc>) = default;
    ~Queue() = default;

    [[nodiscard]] bool empty() const noexcept {
        return data.getSize() == 0;
    }

    [[nodiscard]] size_t size() const noexcept {
        return data.getSize();
    }

    void clear() noexcept {
        data.clear();
    }

    void push(const T& value) {
        data.append(value);
    }

    void push(T&& value) {
        data.append(std::move(value));
    }

    template<typename... Args>
    T& emplace(Args&&... args) {
        data.append_(std::forward<Args>(args)...);
        return back();
    }

    void pop() {
        if (empty()) {
            return;
        }
        const size_t currentSize = data.getSize();
        for (size_t i = 1; i < currentSize; ++i) {
            data[i - 1] = std::move(data[i]);
        }
        data.omit();
    }

    T& front() {
        assert(!empty());
        return data[0];
    }

    const T& front() const {
        assert(!empty());
        return data[0];
    }

    T& back() {
        assert(!empty());
        return data[data.getSize() - 1];
    }

    const T& back() const {
        assert(!empty());
        return data[data.getSize() - 1];
    }

    void swap(Queue& other) noexcept(noexcept(data.swap(other.data))) {
        data.swap(other.data);
    }
};

template<typename T, typename Alloc>
void swap(Queue<T, Alloc>& left, Queue<T, Alloc>& right) noexcept {
    left.swap(right);
}

}

#endif // QUEUE_H
