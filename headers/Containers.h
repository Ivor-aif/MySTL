//
// Created by Ivor_Aif on 2026/1/28.
//

#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <memory>

namespace mySTL::containers::detail {

struct piecewise_construct_t {
    explicit piecewise_construct_t() = default;
};

constexpr piecewise_construct_t piecewiseConstruct{};

template<typename T, typename Alloc = std::allocator<T>>
struct conStorage {
private:
    using allocTraits = std::allocator_traits<Alloc>;

    Alloc alloc;
    size_t size = 0, capacity = 0;
    T* storage = nullptr;

    template<typename... Args>
    void constructAt(T* ptr, Args&&... args) {
        allocTraits::construct(alloc, ptr, std::forward<Args>(args)...);
    }

    void destroyAt(T* ptr) noexcept {
        allocTraits::destroy(alloc, ptr);
    }

    void destroyRange(T* first, T* last) noexcept {
        for (; first != last; ++first) {
            allocTraits::destroy(alloc, first);
        }
    }

public:
    conStorage() : capacity(100) {
        size = 0;
        storage = allocTraits::allocate(alloc, capacity);
    }

    conStorage(const conStorage& other) : size(other.size), capacity(other.capacity) {
        if (capacity == 0) {
            storage = nullptr;
            size = 0;
            return;
        }
        storage = allocTraits::allocate(alloc, capacity);
        size_t i = 0;
        try {
            for ( ; i < size; ++i) {
                constructAt(storage + i, other.storage[i]);
            }
        } catch (...) {
            destroyRange(storage, storage + i);
            allocTraits::deallocate(alloc, storage, capacity);
            storage = nullptr;
            size = 0;
            capacity = 0;
            throw;
        }
    }

    conStorage& operator=(const conStorage& other) {
        if (this == &other) {
            return *this;
        }
        conStorage tmp(other);
        swap(tmp);
        return *this;
    }

    conStorage(conStorage&& other) noexcept(allocTraits::is_always_equal::value)
            : alloc(std::move(other.alloc)), size(other.size), capacity(other.capacity), storage(other.storage) {
        other.size = 0, other.capacity = 0;
        other.storage = nullptr;
    }

    conStorage& operator=(conStorage&& other) noexcept(allocTraits::is_always_equal::value) {
        if (this == &other) {
            return *this;
        }
        conStorage tmp(std::move(other));
        swap(tmp);
        return *this;
    }

    ~conStorage() noexcept {
        if (!storage) {
            return;
        }
        destroyRange(storage, storage + size);
        allocTraits::deallocate(alloc, storage, capacity);
    }

    void reserve(size_t newCap) {
        if (newCap < size) {
            newCap = size;
        }
        if (newCap == capacity) {
            return;
        }
        if (newCap == 0) {
            destroyRange(storage, storage + size);
            allocTraits::deallocate(alloc, storage, capacity);
            storage = nullptr;
            size = 0;
            capacity = 0;
            return;
        }
        T* newStorage = allocTraits::allocate(alloc, newCap);
        size_t i = 0;
        try {
            for (; i < size; ++i) {
                constructAt(newStorage + i, std::move_if_noexcept(storage[i]));
            }
        } catch (...) {
            destroyRange(newStorage, newStorage + i);
            allocTraits::deallocate(alloc, newStorage, newCap);
            throw;
        }
        if (storage) {
            destroyRange(storage, storage + size);
            allocTraits::deallocate(alloc, storage, capacity);
        }
        storage = newStorage;
        capacity = newCap;
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

    void swap(conStorage& other) noexcept {
        auto tmp_alloc = std::move(alloc);
        alloc = std::move(other.alloc);
        other.alloc = std::move(tmp_alloc);
        size_t tmpSize = size;
        size = other.size;
        other.size = tmpSize;
        size_t tmpCap = capacity;
        capacity = other.capacity;
        other.capacity = tmpCap;
        T* tmpPtr = storage;
        storage = other.storage;
        other.storage = tmpPtr;
    }
};

template<typename First, typename Second>
struct KeyValue {
    First first;
    Second second;

    KeyValue() = default;

    KeyValue(const First& first, const Second& second) : first(first), second(second) {

    }

    KeyValue(First&& first, Second&& second) noexcept (std::is_nothrow_move_constructible_v<First> && std::is_nothrow_move_constructible_v<Second>)
            : first(std::move(first)), second(std::move(second)) {

    }

    template<typename Fir, typename Sec, std::enable_if_t< std::is_constructible_v<First, Fir&&> && std::is_constructible_v<Second, Sec&&> && std::is_convertible_v<Fir&&, First> && std::is_convertible_v<Sec&&, Second>>* = nullptr>
    KeyValue(Fir&& first, Sec&& second) : first(std::forward<Fir>(first)), second(std::forward<Sec>(second)) {

    }

    template<typename Fir, typename Sec, std::enable_if_t<std::is_constructible_v<First, Fir&&> && std::is_constructible_v<Second, Sec&&> && (!std::is_convertible_v<Fir&&, First> || !std::is_convertible_v<Sec&&, Second>)>* = nullptr>
    explicit KeyValue(Fir&& first, Sec&& second) : first(std::forward<Fir>(first)), second(std::forward<Sec>(second)) {
    
    }

    template<typename... FirArgs, typename... SecArgs>
    KeyValue(piecewise_construct_t, std::tuple<FirArgs...> firstArgs, std::tuple<SecArgs...> secondArgs) : KeyValue(piecewise_construct_t{},
             firstArgs, secondArgs, std::index_sequence_for<FirArgs...>{}, std::index_sequence_for<SecArgs...>{}) {

    }

    KeyValue(const KeyValue&) = default;

    KeyValue(KeyValue&& other) noexcept (std::is_nothrow_move_constructible_v<First> && std::is_nothrow_move_constructible_v<Second>)
            : first(std::move(other.first)), second(std::move(other.second)) {
                
    }

    KeyValue& operator=(const KeyValue&) = default;

    KeyValue& operator=(KeyValue&& other) noexcept (std::is_nothrow_move_assignable_v<First> && std::is_nothrow_move_assignable_v<Second>) {
        if (this != &other) {
            first = std::move(other.first);
            second = std::move(other.second);
        }
        return *this;
    }

    void swap(KeyValue& other) noexcept (std::is_nothrow_move_constructible_v<First> && std::is_nothrow_move_assignable_v<First> && std::is_nothrow_move_constructible_v<Second> && std::is_nothrow_move_assignable_v<Second>) {
        First tmpFirst(std::move(first));
        first = std::move(other.first);
        other.first = std::move(tmpFirst);
        Second tmpSecond(std::move(second));
        second = std::move(other.second);
        other.second = std::move(tmpSecond);
    }

private:
    template<typename... FirArgs, typename... SecArgs, size_t... Idx1, size_t... Idx2>
    KeyValue(piecewise_construct_t, std::tuple<FirArgs ...>& firstArgs, std::tuple<SecArgs ...>& secondArgs, std::index_sequence<Idx1 ...>, std::index_sequence<Idx2 ...>)
            : first(std::forward<FirArgs>(std::get<Idx1>(firstArgs))...), second(std::forward<SecArgs>(std::get<Idx2>(secondArgs))...) {

    }
};

template<typename First, typename Second>
void swap(KeyValue<First, Second>& left, KeyValue<First, Second>& right) noexcept (std::is_nothrow_move_assignable_v<First> && std::is_nothrow_move_assignable_v<Second>) {
    left.swap(right);
}

template<typename First, typename Second>
bool operator==(const KeyValue<First, Second>& left, const KeyValue<First, Second>& right) {
    return left.first == right.first && left.second == right.second;
}

template<typename First, typename Second>
bool operator!=(const KeyValue<First, Second>& left, const KeyValue<First, Second>& right) {
    return !(left == right);
}

template<typename First, typename Second>
bool operator<(const KeyValue<First, Second>& left, const KeyValue<First, Second>& right) {
    return left.first < right.first || (!(right.first < left.first) && left.second < right.second);
}

template<typename First, typename Second>
bool operator>(const KeyValue<First, Second>& left, const KeyValue<First, Second>& right) {
    return right < left;
}

template<typename First, typename Second>
bool operator<=(const KeyValue<First, Second>& left, const KeyValue<First, Second>& right) {
    return !(right < left);
}

template<typename First, typename Second>
bool operator>=(const KeyValue<First, Second>& left, const KeyValue<First, Second>& right) {
    return !(left < right);
}

template<typename T>
struct lstNode {
public:
    lstNode* last, * next;
    T value;

    template<typename... Args>
    explicit lstNode(Args&&... args) : last(nullptr), next(nullptr), value(std::forward<Args>(args)...) {

    }
};

template<typename T, typename Alloc = std::allocator<T>>
struct lstOps {
    using nodeAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<lstNode<T>>;
    using allocTraits = std::allocator_traits<nodeAlloc>;

    static lstNode<T>* create(nodeAlloc& alloc, const T& value) {
        auto node = allocTraits::allocate(alloc, 1);
        try {
            allocTraits::construct(alloc, node, value);
            return node;
        } catch (...) {
            allocTraits::deallocate(alloc, node, 1);
            throw;
        }
    }

    static void destroy(nodeAlloc& alloc, lstNode<T>* obj) {
        allocTraits::destroy(alloc, obj);
        allocTraits::deallocate(alloc, obj, 1);
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
public:
    lstNode<T>* curr = nullptr;

    explicit lstIterator(lstNode<T>* curr) : curr(curr) {

    } // Only for `List` internal use.

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
public:
    const lstNode<T>* curr = nullptr;

    explicit lstConstIterator(const lstNode<T>* curr) : curr(curr) {

    } // Only for `List` internal use.

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
        lstConstIterator tmp = *this;
        --*this;
        return tmp;
    }

    bool operator==(const lstConstIterator& other) const {
        return curr == other.curr;
    }

    bool operator!=(const lstConstIterator& other) const {
        return curr != other.curr;
    }

    const T* operator->() const {
        return &curr->value;
    }
};

}

#endif // CONTAINERS_H
