//
// Created by Ivor_Aif on 2026/2/17.
//

#ifndef STACK_H
#define STACK_H

#include <cassert>

#include "Containers.h"

namespace mySTL::structures {

template<typename T, typename Alloc = std::allocator<T>>
class Stack {
private:
    containers::detail::conStorage<T, Alloc> data;

public:
    Stack() = default;
    Stack(const Stack&) = default;
    Stack(Stack&&) noexcept(std::is_nothrow_move_constructible_v<Alloc>) = default;
    Stack& operator=(const Stack&) = default;
    Stack& operator=(Stack&&) noexcept(std::is_nothrow_move_assignable_v<Alloc>) = default;
    ~Stack() = default;

    [[nodiscard]] bool empty() const noexcept {
        return data.getSize() == 0;
    }

    [[nodiscard]] size_t getSize() const noexcept {
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
        return top();
    }

    void pop() noexcept {
        if (empty()) {
            return;
        }
        data.omit();
    }

    T& top() {
        assert(!empty());
        return data[data.getSize() - 1];
    }

    const T& top() const {
        assert(!empty());
        return data[data.getSize() - 1];
    }

    void swap(Stack& other) noexcept(noexcept(data.swap(other.data))) {
        data.swap(other.data);
    }
};

template<typename T, typename Alloc>
void swap(Stack<T, Alloc>& left, Stack<T, Alloc>& right) noexcept {
    left.swap(right);
}

}

#endif // STACK_H
