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
    typename std::allocator_traits<Alloc>::template rebind_alloc<detail::lstNode<T>> alloc;

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
            detail::lstOps<T, Alloc>::destroy(alloc, curr);
            curr = nxt;
        }
        sentinel->last = sentinel;
        sentinel->next = sentinel;
        len = 0;
    }

    detail::lstIterator<T> insert(detail::lstIterator<T> pos, const T& value, const bool front = true) {
        detail::lstNode<T>* curr = pos.curr, * node = detail::lstOps<T, Alloc>::create(alloc, value);
        if (front) {
            detail::lstOps<T, Alloc>::link(curr->last, node);
            detail::lstOps<T, Alloc>::link(node, curr);
        } else {
            detail::lstOps<T, Alloc>::link(node, curr->next);
            detail::lstOps<T, Alloc>::link(curr, node);
        }
        len++;
        return detail::lstIterator<T>(node);
    }

    detail::lstIterator<T> erase(detail::lstIterator<T> pos) noexcept {
        if (pos.curr == sentinel) {
            return end();
        }
        detail::lstNode<T>* node = pos.curr, * nxt = pos.curr->next;
        detail::lstOps<T, Alloc>::unlink(node);
        detail::lstOps<T, Alloc>::destroy(alloc, node);
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
        return detail::lstIterator<T>(sentinel->next);
    }

    detail::lstIterator<T> end() {
        return detail::lstIterator<T>(sentinel);
    }

    detail::lstConstIterator<T> begin() const {
        return detail::lstConstIterator<T>(sentinel->next);
    }

    detail::lstConstIterator<T> end() const {
        return detail::lstConstIterator<T>(sentinel);
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

    void splice(detail::lstIterator<T> pos, List& other, detail::lstIterator<T> it) noexcept {
        if (this == &other || it.curr == other.sentinel) {
            return;
        }
        auto node = it.curr;
        detail::lstOps<T, Alloc>::unlink(node);
        --other.len;
        auto cur = pos.curr;
        detail::lstOps<T, Alloc>::link(cur->last, node);
        detail::lstOps<T, Alloc>::link(node, cur);
        ++len;
    }

    void splice(detail::lstIterator<T> pos, List& other) noexcept {
        if (other.empty()) {
            return;
        }
        auto head = other.sentinel->next, tail = other.sentinel->last;
        other.sentinel->next = other.sentinel;
        other.sentinel->last = other.sentinel;
        auto cur = pos.curr;
        detail::lstOps<T, Alloc>::link(cur->last, head);
        detail::lstOps<T, Alloc>::link(tail, cur);
        len += other.len;
        other.len = 0;
    }

    void merge(List& other) {
        merge(other, std::less<T>{});
    }

    template<typename Compare>
    void merge(List& other, Compare cmp) {
        if (this == &other || other.empty()) {
            return;
        }
        detail::lstIterator<T> it1 = begin(), it2 = other.begin();
        while (it1 != end() && it2 != other.end()) {
            if (cmp(*it2, *it1)) {
                detail::lstIterator<T> next = it2;
                ++next;
                splice(it1, other, it2);
                it2 = next;
            } else {
                ++it1;
            }
        }
        if (it2 != other.end()) {
            splice(end(), other);
        }
    }

    void splice(detail::lstIterator<T> pos, List& other, detail::lstIterator<T> head, detail::lstIterator<T> tail) noexcept {
        if (head == tail || this == &other && pos == head) {
            return;
        }
        size_t nn = 0;
        for (auto it = head; it != tail; ++it) {
            nn++;
        }
        detail::lstNode<T>* node1 = head.curr, * node2 = tail.curr->last;
        node1->last->next = node2->next;
        node2->next->last = node1->last;
        detail::lstNode<T>* cur = pos.curr;
        node1->last = cur->last;
        node2->next = cur;
        cur->last->next = node1;
        cur->last = node2;
        len += nn;
        other.len -= nn;
    }

    void sort() {
        sort(std::less<T>{});
    }

    template<typename Compare>
    void sort(Compare comp) {
        if (len < 2) {
            return;
        }
        List second;
        detail::lstIterator<T> slow = begin(), fast = begin();
        while (fast != end()) {
            ++fast;
            if (fast != end()) {
                ++slow;
                ++fast;
            }
        }
        second.splice(second.begin(), *this, slow, end());
        this->sort(comp);
        second.sort(comp);
        this->merge(second, comp);
    }
};

}

#endif // LIST_H
