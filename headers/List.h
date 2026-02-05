//
// Created by Ivor_Aif on 2026/2/5.
//

#ifndef LIST_H
#define LIST_H

#include <cassert>
#include <iterator>

#include "Containers.h"

namespace mySTL::containers {

template<typename T, typename Alloc = std::allocator<T>>
class List {
private:
    detail::lstNode<T>* sentinel;
    size_t len = 0;
    std::allocator_traits<typename std::allocator_traits<Alloc>::template rebind_alloc<detail::lstNode<T>>> alloc;

public:
    List() {
        sentinel = static_cast<detail::lstNode<T>*>(operator new(sizeof(detail::lstNode<T>)));
        sentinel->next = sentinel;
        sentinel->last = sentinel;
    }

    List(const List& other) : List() {
        for (const auto& element : other) {
            pushBack(element);
        }
    }

    List& operator=(List other) {
        swap(other);
        return *this;
    }

    ~List() noexcept {
        clear();
        operator delete(sentinel);
    }

    void swap(List& other) noexcept {
        std::swap(sentinel, other.sentinel);
        std::swap(len, other.len);
    }

    List(List&& other) noexcept : List() {
        swap(other);
    }

    List& operator=(List&& other) noexcept {
        if (this != &other) {
            clear();
            operator delete(sentinel);
            sentinel = other.sentinel;
            len = other.len;
            other.sentinel = nullptr;
            other.len = 0;
        }
        return *this;
    }

    List& operator=(const List&) = delete;

    [[nodiscard]] size_t length() const {
        return len;
    }

    [[nodiscard]] bool empty() const {
        return len == 0;
    }

    void clear() {
        detail::lstNode<T>* curr = sentinel->next;
        while (curr != sentinel) {
            detail::lstNode<T>* nxt = curr->next;
            detail::lstOps<T>::destroy(curr);
            curr = nxt;
        }
        sentinel->last = sentinel;
        sentinel->next = sentinel;
        len = 0;
    }

    detail::lstIterator<T> insert(detail::lstIterator<T> pos, const T& value, const bool front = true) {
        detail::lstNode<T>* curr = pos.curr, * node = detail::lstOps<T>::create(value);
        if (front) {
            detail::lstOps<T>::link(curr->last, node);
            detail::lstOps<T>::link(node, curr);
        } else {
            detail::lstOps<T>::link(node, curr->next);
            detail::lstOps<T>::link(curr, node);
        }
        len++;
        return detail::lstIterator<T>{ node };
    }

    detail::lstIterator<T> erase(detail::lstIterator<T> pos) {
        if (pos.curr == sentinel) {
            return end();
        }
        detail::lstNode<T>* node = pos.curr, * nxt = pos.curr->next;
        detail::lstOps<T>::unlink(node);
        detail::lstOps<T>::destroy(node);
        len--;
        return detail::lstIterator<T>{ nxt };
    }

    detail::lstIterator<T> find(const T& value) {
        for (auto it = begin(); it != end(); ++it) {
            if (*it == value) {
                return it;
            }
        }
        return end();
    }

    detail::lstConstIterator<T> find(const T& value) const {
        for (auto it = begin(); it != end(); ++it) {
            if (*it == value) {
                return it;
            }
        }
        return endConst();
    }

    void pushBack(const T& value) {
        insert(end(), value);
    }

    void popBack() {
        if (!empty()) {
            erase(detail::lstIterator<T>{ sentinel->last });
        }
    }

    void pushFront(const T& value) {
        insert(begin(), value);
    }

    void popFront() {
        if (!empty()) {
            erase(begin());
        }
    }

    detail::lstIterator<T> begin() {
        return detail::lstIterator<T>{ sentinel->next };
    }

    detail::lstIterator<T> end() {
        return detail::lstIterator<T>{ sentinel };
    }

    detail::lstConstIterator<T> begin() const {
        return detail::lstConstIterator<T>{ sentinel->next };
    }

    detail::lstConstIterator<T> end() const {
        return detail::lstConstIterator<T>{ sentinel };
    }

    detail::lstConstIterator<T> beginConst() const {
        return begin();
    }

    detail::lstConstIterator<T> endConst() const {
        return end();
    }

    T& front() const {
        assert(!empty());
        return sentinel->next->value;
    }

    T& back() const {
        assert(!empty());
        return sentinel->last->value;
    }

    void splice(detail::lstIterator<T> pos, List& other, detail::lstIterator<T> it) {
        if (this == &other || it.curr == other.sentinel) {
            return;
        }
        auto node = it.curr;
        detail::lstOps<T>::unlink(node);
        --other.len;
        auto cur = pos.curr;
        detail::lstOps<T>::link(cur->last, node);
        detail::lstOps<T>::link(node, cur);
        ++len;
    }

    void splice(detail::lstIterator<T> pos, List& other) {
        if (other.empty()) {
            return;
        }
        auto head = other.sentinel->next, tail = other.sentinel->last;
        other.sentinel->next = other.sentinel;
        other.sentinel->last = other.sentinel;
        auto cur = pos.curr;
        detail::lstOps<T>::link(cur->last, head);
        detail::lstOps<T>::link(tail, cur);
        len += other.len;
        other.len = 0;
    }

    void merge(List& other) {
        if (this == &other || other.empty()) {
            return;
        }
        auto it1 = begin(), it2 = other.begin();
        while (it1 != end() && it2 != other.end()) {
            if (*it2 < *it1) {
                auto nxt = it2;
                ++nxt;
                splice(it1, other, it2);
                it2 = nxt;
            } else {
                ++it1;
            }
        }
        if (it2 != other.end()) {
            splice(end(), other);
        }
    }

    void splice(detail::lstIterator<T> pos, List& other, detail::lstIterator<T> head, detail::lstIterator<T> tail) {
        if (head == tail || this == &other && pos == head) {
            return;
        }
        detail::lstNode<T>* node1 = head.curr, * node2 = tail.curr->last;
        node1->last->next = node2->next;
        node2->next->last = node1->last;
        detail::lstNode<T>* cur = pos.curr;
        node1->last = cur->last;
        node2->next = cur;
        cur->last->next = node1;
        cur->last = node2;
        size_t nn = 0;
        for (auto it = head; it != tail; ++it) {
            nn++;
        }
        len += nn;
        other.len -= nn;
    }

    void sort() {
        if (len < 2) {
            return;
        }
        List second;
        auto slow = begin(), fast = begin();
        while (fast != end()) {
            ++fast;
            if (fast != end()) {
                ++slow;
                ++fast;
            }
        }
        second.splice(second.begin(), *this, slow, end());
        this->sort();
        second.sort();
        this->merge(second);
    }
};

}

#endif // LIST_H
