//
// Created by Ivor_Aif on 2026/1/29.
//

#ifndef ARRAY_H
#define ARRAY_H

#include "Containers.h"

namespace mySTL::containers {

template<typename T>
class Array {
private:
    detail::conStorage<T> data;

public:
    Array() = default;

    Array(const size_t nn, const T& value) {
        data.reserve(nn);
        for (size_t i = 0; i < nn; ++i) {
            data.append(value);
        }
    }

    Array(std::initializer_list<T> init) {
        data.reserve(init.size());
        for (const T& value : init) {
            data.append(value);
        }
    }

    Array(Array&&) noexcept = default;
    Array& operator=(Array&&) noexcept = default;
    Array(const Array&) = default;
    Array& operator=(const Array&) = default;
    ~Array() = default;

    [[nodiscard]] size_t getSize() const noexcept {
        return data.getSize();
    }

    T* getData() noexcept {
        return data.getData();
    }

    const T* getData() const noexcept {
        return data.getData();
    }

    [[nodiscard]] bool empty() const noexcept {
        return data.getSize() == 0;
    }

    void clear() noexcept {
        data.clear();
    }

    T& operator[](size_t index) noexcept {
        return data[index];
    }

    const T& operator[](size_t index) const noexcept {
        return data[index];
    }

    void pushBack(const T& value) {
        data.append(value);
    }

    void pushBack(T&& value) {
        data.append(std::move(value));
    }

    template<typename... Args>
    void emplaceBack(Args&&... args) {
        data.append_(std::forward<Args>(args)...);
    }

    T& get(size_t index) {
        if (index >= data.getSize()) {
            throw std::out_of_range("Array::get");
        }
        return (*this)[index];
    }

    const T& get(size_t index) const {
        if (index >= data.getSize()) {
            throw std::out_of_range("Array::get");
        }
        return (*this)[index];
    }

    void popBack() noexcept {
        data.omit();
    }

    T* begin() noexcept {
        return data.getData();
    }

    const T* begin() const noexcept {
        return data.getData();
    }

    T* end() noexcept {
        return data.getData() + data.getSize();
    }

    const T* end() const noexcept {
        return data.getData() + data.getSize();;
    }

    T& front() noexcept {
        return data[0];
    }
    const T& front() const noexcept {
        return data[0];
    }

    T& back() noexcept {
        return data[getSize() - 1];
    }

    const T& back() const noexcept {
        return data[getSize() - 1];
    }

    [[nodiscard]] size_t capacity() const noexcept {
        return data.getCapacity();
    }

    void reserve(size_t nn) {
        if (nn > data.getCapacity()) {
            data.reserve(nn);
        }
    }

    void shrink() noexcept {
        data.shrink();
    }

    void swap(Array& other) noexcept {
        data.swap(other.data);
    }
};

}

#endif // ARRAY_H
