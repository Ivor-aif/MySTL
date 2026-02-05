//
// Created by Ivor_Aif on 2026/1/28.
//

#ifndef CONTAINERS_H
#define CONTAINERS_H

namespace mySTL::containers::detail {

template<typename T, typename Alloc = std::allocator<T>>
struct conStorage {
private:
    Alloc alloc;
    size_t size = 0, capacity = 0;
    T* storage = nullptr;
    void reserve(size_t newCap) {
        if (newCap == capacity) {
            return;
        }
        T* newStorage = std::allocator_traits<Alloc>::allocate(alloc, newCap);
        size_t i = 0;
        try {
            for (; i < size; ++i) {
                constructAt(newStorage + i, std::move_if_noexcept(storage[i]));
            }
        } catch (...) {
            destroyRange(newStorage, newStorage + i);
            std::allocator_traits<Alloc>::deallocate(alloc, newStorage, newCap);
            throw;
        }
        destroyRange(storage, storage + size);
        std::allocator_traits<Alloc>::deallocate(alloc, storage, capacity);
        storage = newStorage;
        capacity = newCap;
    }

    template<typename... Args>
    void constructAt(T* ptr, Args&&... args) {
        std::allocator_traits<Alloc>::construct(alloc, ptr, std::forward<Args>(args)...);
    }

    void destroyAt(T* ptr) noexcept {
        std::allocator_traits<Alloc>::destroy(alloc, ptr);
    }

    void destroyRange(T* first, T* last) noexcept {
        for (; first != last; ++first) {
            std::allocator_traits<Alloc>::destroy(alloc, first);
        }
    }

public:
    conStorage() : capacity(100) {
        size = 0;
        storage = std::allocator_traits<Alloc>::allocate(alloc, capacity);
    }

    conStorage(const conStorage&) = delete;
    conStorage& operator=(const conStorage&) = delete;

    conStorage(conStorage&& other) noexcept(std::allocator_traits<Alloc>::is_always_equal::value)
            : alloc(std::move(other.alloc)), size(other.size), capacity(other.capacity), storage(other.storage) {
        other.size = 0, other.capacity = 0;
        other.storage = nullptr;
    }

    conStorage& operator=(conStorage&& other) = delete;

    ~conStorage() noexcept {
        destroyRange(storage, storage + size);
        std::allocator_traits<Alloc>::deallocate(alloc, storage, capacity);
    }

    [[nodiscard]] size_t getSize() const noexcept {
        return size;
    }

    [[nodiscard]] size_t getCapacity() const noexcept {
        return capacity;
    }

    T* getData() noexcept {
        return storage;
    }

    const T* getData() const noexcept {
        return storage;
    }

    T& operator[](size_t i) noexcept {
        return storage[i];
    }

    const T& operator[](size_t i) const noexcept {
        return storage[i];
    }

    template<typename... Args>
    void append_(Args&&... args) {
        if (size == capacity) {
            reserve(capacity ? capacity << 1 : 100);
        }
        constructAt(storage + size, std::forward<Args>(args)...);
        ++size;
    }

    void append(const T& element) {
        append_(element);
    }

    void append(T&& element) {
        append_(std::move(element));
    }

    void omit() noexcept {
        if (!size) {
            return;
        }
        destroyAt(storage + --size);
    }

    void clear() noexcept {
        destroyRange(storage, storage + size);
        size = 0;
    }

    void shrink() {
        if (size == capacity) {
            return;
        }
        reserve(size);
    }
};

template<typename T>
struct lstNode {
public:
    lstNode* last, * next;
    T value;

private:
    template<typename> friend struct lstOps;

    template<typename... Args>
    explicit lstNode(Args&&... args) : last(nullptr), next(nullptr), value(std::forward<Args>(args)...) {

    }
};

template<typename T, typename Alloc = std::allocator<T>>
struct lstOps {
    static lstNode<T>* create(typename std::allocator_traits<Alloc>::template rebind_alloc<lstNode<T>>& alloc, const T& value) {
        auto node = std::allocator_traits<typename std::allocator_traits<Alloc>::template rebind_alloc<lstNode<T>>>::allocate(alloc, 1);
        try {
            std::allocator_traits<typename std::allocator_traits<Alloc>::template rebind_alloc<lstNode<T>>>::construct(alloc, node, value);
            return node;
        } catch (...) {
            std::allocator_traits<typename std::allocator_traits<Alloc>::template rebind_alloc<lstNode<T>>>::deallocate(alloc, node, 1);
            throw;
        }
    }

    static void destroy(typename std::allocator_traits<Alloc>::template rebind_alloc<lstNode<T>>& alloc, lstNode<T>* obj) {
        std::allocator_traits<typename std::allocator_traits<Alloc>::template rebind_alloc<lstNode<T>>>::destroy(alloc, obj);
        std::allocator_traits<typename std::allocator_traits<Alloc>::template rebind_alloc<lstNode<T>>>::deallocate(alloc, obj, 1);
    }

    static void link(lstNode<T>* first, lstNode<T>* second) {
        if (first && second) {
            first->next = second;
            second->last = first;
        }
    }

    static void unlink(lstNode<T>* node) {
        link(node->last, node->next);
        node->last = node->next = nullptr;
    }
};

template<typename T>
class lstIterator {
private:
    lstNode<T>* curr;
    template<typename> friend class List;

public:
    lstIterator() : curr(nullptr) {

    }

    T& operator*() const {
        return curr->value;
    }

    lstIterator& operator++() {
        curr = curr->next;
        return *this;
    }

    lstIterator operator++(int) {
        lstIterator tmp = *this;
        ++*this;
        return tmp;
    }

    lstIterator& operator--() {
        curr = curr->last;
        return *this;
    }

    lstIterator operator--(int) {
        lstIterator tmp = *this;
        --*this;
        return tmp;
    }

    bool operator==(const lstIterator& other) const {
        return curr == other.curr;
    }

    bool operator!=(const lstIterator& other) const {
        return curr != other.curr;
    }

    T* operator->() const {
        return &curr->value;
    }
};

template<typename T>
class lstConstIterator {
private:
    const lstNode<T>* curr;
    template<typename> friend class List;

public:
    lstConstIterator() : curr(nullptr) {

    }

    explicit lstConstIterator(const lstIterator<T>& it) : curr(it.curr) {

    }

    const T& operator*() const {
        return curr->value;
    }

    lstConstIterator& operator++() {
        curr = curr->next;
        return *this;
    }

    lstConstIterator operator++(int) {
        lstConstIterator tmp = *this;
        ++*this;
        return tmp;
    }

    lstConstIterator& operator--() {
        curr = curr->last;
        return *this;
    }

    lstConstIterator operator--(int) {
        lstIterator tmp = *this;
        --*this;
        return tmp;
    }

    bool operator==(const lstConstIterator& other) const {
        return curr == other.curr;
    }

    bool operator!=(const lstConstIterator& other) const {
        return curr != other.curr;
    }

    T* operator->() const {
        return &curr->value;
    }
};

}

#endif // CONTAINERS_H
