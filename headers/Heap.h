//
// Created by Ivor_Aif on 2026/2/28.
//

#ifndef HEAP_H
#define HEAP_H

#include "Array.h"

namespace mySTL::structures {

template<typename T, typename Compare = std::less<T>>
class Heap {
private:
    containers::Array<T> data;
    Compare comp;

    void siftUp(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (comp(data[parent], data[index])) {
                swapData(parent, index);
                index = parent;
            } else {
                break;
            }
        }
    }
    
    void swapData(size_t i, size_t j) {
        T temp = std::move(data[i]);
        data[i] = std::move(data[j]);
        data[j] = std::move(temp);
    }

    void siftDown(size_t index) {
        const size_t size = data.getSize();
        while (true) {
            size_t left = 2 * index + 1;
            size_t right = 2 * index + 2;
            size_t largest = index;
            if (left < size && comp(data[largest], data[left])) {
                largest = left;
            }
            if (right < size && comp(data[largest], data[right])) {
                largest = right;
            }
            if (largest != index) {
                swapData(index, largest);
                index = largest;
            } else {
                break;
            }
        }
    }

public:
    Heap() = default;

    explicit Heap(const Compare& compare) : comp(compare) {

    }

    Heap(std::initializer_list<T> init, const Compare& compare = Compare()) : data(init), comp(compare) {
        if (data.getSize() > 1) {
            for (size_t i = data.getSize() / 2; i > 0; --i) {
                siftDown(i - 1);
            }
            siftDown(0);
        }
    }

    explicit Heap(const containers::Array<T>& array, const Compare& compare = Compare()) : data(array), comp(compare) {
        if (data.getSize() > 1) {
            for (size_t i = data.getSize() / 2; i > 0; --i) {
                siftDown(i - 1);
            }
            siftDown(0);
        }
    }

    explicit Heap(containers::Array<T>&& array, const Compare& compare = Compare()) : data(std::move(array)), comp(compare) {
        if (data.getSize() > 1) {
            for (size_t i = data.getSize() / 2; i > 0; --i) {
                siftDown(i - 1);
            }
            siftDown(0);
        }
    }

    ~Heap() = default;

    [[nodiscard]] size_t getSize() const noexcept {
        return data.getSize();
    }

    [[nodiscard]] bool empty() const noexcept {
        return data.empty();
    }

    [[nodiscard]] size_t capacity() const noexcept {
        return data.capacity();
    }

    void reserve(size_t n) {
        data.reserve(n);
    }

    void shrink() noexcept {
        data.shrink();
    }

    void clear() noexcept {
        data.clear();
    }

    const T* getData() const noexcept {
        return data.getData();
    }
    
    const T* begin() const noexcept {
        return data.begin();
    }

    const T* end() const noexcept {
        return data.end();
    }

    const T& operator[](size_t index) const noexcept {
        return data[index];
    }

    void push(const T& value) {
        data.pushBack(value);
        siftUp(data.getSize() - 1);
    }

    void push(T&& value) {
        data.pushBack(std::move(value));
        siftUp(data.getSize() - 1);
    }

    template<typename... Args>
    void emplace(Args&&... args) {
        data.emplaceBack(std::forward<Args>(args)...);
        siftUp(data.getSize() - 1);
    }

    void pop() {
        if (data.empty()) return;
        swapData(0, data.getSize() - 1);
        data.popBack();
        if (!data.empty()) {
            siftDown(0);
        }
    }

    [[nodiscard]] const T& top() const {
        if (data.empty()) {
            throw std::out_of_range("Heap::top");
        }
        return data.front();
    }

    const containers::Array<T>& getArray() const noexcept {
        return data;
    }
};

}

#endif // HEAP_H
