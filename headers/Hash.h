//
// Created by Ivor_Aif on 2026/2/16.
//

#ifndef HASH_H
#define HASH_H

#include "Array.h"
#include "List.h"

namespace mySTL::containers {

template<typename Key, typename T, typename HashFn = std::hash<Key>, typename KeyEqual = std::equal_to<Key>, typename Alloc = std::allocator<detail::KeyValue<const Key, T>>>
class Hash {
private:
    using VT = detail::KeyValue<const Key, T>;

    Array<List<VT, Alloc>> buckets;
    size_t elemCount = 0;
    double maxLoadFactor = 1.;
    HashFn hashFn;
    KeyEqual keyEq;
    Alloc alloc;

    size_t bucketIndex(const Key& key, const size_t bucketCount) const {
        return bucketCount ? static_cast<size_t>(hashFn(key) & bucketCount - 1) : 0;
    }

    size_t bucketIndex(const Key& key) const {
        return bucketIndex(key, buckets.getSize());
    }

    void clearBuckets() noexcept {
        for (size_t i = 0; i < buckets.getSize(); ++i) {
            buckets[i].clear();
        }
        elemCount = 0;
    }

    void rehashIfNeeded() {
        const size_t bucketCountValue = buckets.getSize();
        if (!bucketCountValue) {
            rehash(16);
            return;
        }
        if (static_cast<double>(elemCount + 1) > maxLoadFactor * static_cast<double>(bucketCountValue)) {
            rehash(bucketCountValue << 1);
        }
    }

    void moveFrom(Hash& other) noexcept {
        buckets.swap(other.buckets);
        std::swap(elemCount, other.elemCount);
        std::swap(maxLoadFactor, other.maxLoadFactor);
        std::swap(hashFn, other.hashFn);
        std::swap(keyEq, other.keyEq);
        if constexpr (std::allocator_traits<Alloc>::propagate_on_container_move_assignment::value) {
            alloc = std::move(other.alloc);
        }
    }

public:
    class Iterator;
    class ConstIterator;
    using InsertResult = detail::KeyValue<Iterator, bool>;

    Hash() : buckets(16, List<VT, Alloc>()) {

    }

    explicit Hash(size_t bucketCount, const HashFn& hf = HashFn(), const KeyEqual& eq = KeyEqual(), const Alloc& a = Alloc()) : buckets(bucketCount ? bucketCount : 16, List<VT, Alloc>()), hashFn(hf), keyEq(eq), alloc(a) {
    
    }

    Hash(std::initializer_list<VT> init, size_t bucketCount = 0, const HashFn& hf = HashFn(), const KeyEqual& eq = KeyEqual(), const Alloc& alloc = Alloc()) : Hash(bucketCount ? bucketCount : init.getSize(), hf, eq, alloc) {
        for (const auto& val : init) {
            insert(val);
        }
    }

    template<typename InputIt>
    Hash(InputIt first, InputIt last, size_t bucketCount = 0, const HashFn& hf = HashFn(), const KeyEqual& eq = KeyEqual(), const Alloc& alloc = Alloc()) : Hash(bucketCount ? bucketCount : static_cast<size_t>(std::distance(first, last)), hf, eq, alloc) {
        for ( ; first != last; ++first) {
            insert(*first);
        }
    }

    Hash(const Hash& other) : buckets(other.buckets.getSize() ? other.buckets.getSize() : 16, List<VT, Alloc>()), maxLoadFactor(other.maxLoadFactor), hashFn(other.hashFn), keyEq(other.keyEq), alloc(std::allocator_traits<Alloc>::select_on_container_copy_construction(other.alloc)) {
        for (const auto& val : other) {
            insert(val);
        }
    }

    Hash(Hash&& other) noexcept : buckets(0, List<VT, Alloc>()), hashFn(other.hashFn), keyEq(other.keyEq), alloc(std::move(other.alloc)) {
        moveFrom(other);
    }

    Hash& operator=(Hash other) noexcept(std::allocator_traits<Alloc>::propagate_on_container_move_assignment::value && std::allocator_traits<Alloc>::is_always_equal::value) {
        swap(other);
        return *this;
    }

    ~Hash() noexcept {
        clearBuckets();
    }

    void swap(Hash& other) noexcept(std::allocator_traits<Alloc>::propagate_on_container_swap::value || std::allocator_traits<Alloc>::is_always_equal::value) {
        buckets.swap(other.buckets);
        std::swap(elemCount, other.elemCount);
        std::swap(maxLoadFactor, other.maxLoadFactor);
        std::swap(hashFn, other.hashFn);
        std::swap(keyEq, other.keyEq);
        if constexpr (std::allocator_traits<Alloc>::propagate_on_container_swap::value) {
            std::swap(alloc, other.alloc);
        }
    }

    [[nodiscard]] size_t getSize() const noexcept {
        return elemCount;
    }

    [[nodiscard]] bool isEmpty() const noexcept {
        return elemCount == 0;
    }

    [[nodiscard]] size_t getBucketCount() const noexcept {
        return buckets.getSize();
    }

    [[nodiscard]] double getLoadFactor() const noexcept {
        const size_t bc = buckets.getSize();
        if (!bc) {
            return 0.;
        }
        return static_cast<double>(elemCount) / static_cast<double>(bc);
    }

    void setMaxLoadFactor(const double lf) {
        if (lf <= 0.) {
            throw std::invalid_argument("Hash::setMaxLoadFactor");
        }
        maxLoadFactor = lf;
    }

    [[nodiscard]] double getMaxLoadFactor() const noexcept {
        return maxLoadFactor;
    }

    void clear() noexcept {
        clearBuckets();
    }

    void reserve(const size_t count) {
        const size_t bc = buckets.getSize();
        const double lf = maxLoadFactor;
        if (const size_t required = static_cast<size_t>(static_cast<double>(count) / lf) + 1; required > bc) {
            rehash(required);
        }
    }

    void rehash(size_t newBucketCount) {
        if (newBucketCount == 0) {
            newBucketCount = 16;
        }
        Array<List<VT, Alloc>> newBuckets(newBucketCount, List<VT, Alloc>());
        for (size_t i = 0; i < buckets.getSize(); ++i) {
            List<VT, Alloc>& bucket = buckets[i];
            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                auto idx = static_cast<size_t>(hashFn(it->first) % newBucketCount);
                newBuckets[idx].pushBack(std::move(*it));
            }
        }
        buckets.swap(newBuckets);
    }

    class Iterator {
        friend class Hash;

    private:
        Hash* owner = nullptr;
        size_t index = 0;
        detail::lstIterator<VT> it;

        Iterator(Hash* owner, const size_t index, detail::lstIterator<VT> it) : owner(owner), index(index), it(it) {
        
        }

        void advance() {
            if (!owner) {
                return;
            }
            const size_t bucketSize = owner->buckets.getSize();
            if (index >= bucketSize) {
                return;
            }
            if (List<VT, Alloc>& bucket = owner->buckets[index]; it != bucket.end()) {
                ++it;
                if (it != bucket.end()) {
                    return;
                }
            }
            for (size_t i = index + 1; i < bucketSize; ++i) {
                if (List<VT, Alloc>& nextBucket = owner->buckets[i]; !nextBucket.empty()) {
                    index = i;
                    it = nextBucket.begin();
                    return;
                }
            }
            index = bucketSize;
            it = detail::lstIterator<VT>{};
        }

    public:
        Iterator() = default;

        VT& operator*() const {
            return *it;
        }

        VT* operator->() const {
            return &*it;
        }

        Iterator& operator++() {
            advance();
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp(*this);
            advance();
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return owner == other.owner && index == other.index && it == other.it;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    class ConstIterator {
        friend class Hash;

    private:
        const Hash* owner = nullptr;
        size_t index = 0;
        detail::lstConstIterator<VT> it;

        ConstIterator(const Hash* owner, const size_t index, detail::lstConstIterator<VT> it) : owner(owner), index(index), it(it) {
        }

        void advance() {
            if (!owner) {
                return;
            }
            const size_t bucketSize = owner->buckets.getSize();
            if (index >= bucketSize) {
                return;
            }
            if (const List<VT, Alloc>& bucket = owner->buckets[index]; it != bucket.endConst()) {
                ++it;
                if (it != bucket.endConst()) {
                    return;
                }
            }
            for (size_t i = index + 1; i < bucketSize; ++i) {
                if (const List<VT, Alloc>& nextBucket = owner->buckets[i]; !nextBucket.empty()) {
                    index = i;
                    it = nextBucket.beginConst();
                    return;
                }
            }
            index = bucketSize;
            it = detail::lstConstIterator<VT>{};
        }

    public:
        ConstIterator() = default;

        explicit ConstIterator(const Iterator& other) : owner(other.owner), index(other.index), it(other.it.curr ? detail::lstConstIterator<VT>(other.it) : detail::lstConstIterator<VT>()) {
        
        }

        const VT& operator*() const {
            return *it;
        }

        const VT* operator->() const {
            return &*it;
        }

        ConstIterator& operator++() {
            advance();
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator tmp(*this);
            advance();
            return tmp;
        }

        bool operator==(const ConstIterator& other) const {
            return owner == other.owner && index == other.index && it == other.it;
        }

        bool operator!=(const ConstIterator& other) const {
            return !(*this == other);
        }
    };

    Iterator begin() noexcept {
        const size_t bucketSize = buckets.getSize();
        for (size_t i = 0; i < bucketSize; ++i) {
            if (List<VT, Alloc>& bucket = buckets[i]; !bucket.empty()) {
                return Iterator(this, i, bucket.begin());
            }
        }
        return end();
    }

    Iterator end() noexcept {
        return Iterator(this, buckets.getSize(), detail::lstIterator<VT>{});
    }

    ConstIterator begin() const noexcept {
        return cBegin();
    }

    ConstIterator end() const noexcept {
        return cEnd();
    }

    ConstIterator cBegin() const noexcept {
        const size_t bucketSize = buckets.getSize();
        for (size_t i = 0; i < bucketSize; ++i) {
            if (const List<VT, Alloc>& bucket = buckets[i]; !bucket.empty()) {
                return ConstIterator(this, i, bucket.beginConst());
            }
        }
        return cEnd();
    }

    ConstIterator cEnd() const noexcept {
        return ConstIterator(this, buckets.getSize(), detail::lstConstIterator<VT>{});
    }

    InsertResult insert(const VT& value) {
        rehashIfNeeded();
        size_t index = bucketIndex(value.first);
        List<VT, Alloc>& bucket = buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (keyEq(it->first, value.first)) {
                return { Iterator(this, index, it), false };
            }
        }
        bucket.pushBack(value);
        ++elemCount;
        auto it = bucket.end();
        --it;
        return { Iterator(this, index, it), true };
    }

    InsertResult insert(VT&& value) {
        rehashIfNeeded();
        size_t index = bucketIndex(value.first);
        List<VT, Alloc>& bucket = buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (keyEq(it->first, value.first)) {
                return { Iterator(this, index, it), false };
            }
        }
        bucket.pushBack(std::move(value));
        ++elemCount;
        auto it = bucket.end();
        --it;
        return { Iterator(this, index, it), true };
    }

    template<typename... Args>
    InsertResult emplace(Args&&... args) {
        VT value(std::forward<Args>(args)...);
        return insert(std::move(value));
    }

    T& operator[](const Key& key) {
        auto it = find(key);
        if (it != end()) {
            return it->second;
        }
        return emplace(key, T()).first->second;
    }

    T& get(const Key& key) {
        auto it = find(key);
        if (it == end()) {
            throw std::out_of_range("Hash::get");
        }
        return it->second;
    }

    const T& get(const Key& key) const {
        auto it = find(key);
        if (it == end()) {
            throw std::out_of_range("Hash::get");
        }
        return it->second;
    }

    Iterator find(const Key& key) noexcept {
        if (const size_t bucketSize = buckets.getSize(); !bucketSize) {
            return end();
        }
        size_t index = bucketIndex(key);
        List<VT, Alloc>& bucket = buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (keyEq(it->first, key)) {
                return Iterator(this, index, it);
            }
        }
        return end();
    }

    ConstIterator find(const Key& key) const noexcept {
        if (const size_t bucketSize = buckets.getSize(); !bucketSize) {
            return cEnd();
        }
        size_t index = bucketIndex(key);
        const List<VT, Alloc>& bucket = buckets[index];
        for (auto it = bucket.beginConst(); it != bucket.endConst(); ++it) {
            if (keyEq(it->first, key)) {
                return ConstIterator(this, index, it);
            }
        }
        return cEnd();
    }

    bool contains(const Key& key) const noexcept {
        return find(key) != cEnd();
    }

    size_t erase(const Key& key) noexcept {
        if (const size_t bucketSize = buckets.getSize(); !bucketSize) {
            return 0;
        }
        size_t index = bucketIndex(key);
        List<VT, Alloc>& bucket = buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (keyEq(it->first, key)) {
                bucket.erase(it);
                --elemCount;
                return 1;
            }
        }
        return 0;
    }

    Iterator erase(Iterator pos) noexcept {
        if (pos == end()) {
            return pos;
        }
        size_t index = pos.index;
        List<VT, Alloc>& bucket = buckets[index];
        detail::lstIterator<VT> nextIt = bucket.erase(pos.it);
        --elemCount;
        if (nextIt != bucket.end()) {
            return Iterator(this, index, nextIt);
        }
        const size_t bucketSize = buckets.getSize();
        for (size_t i = index + 1; i < bucketSize; ++i) {
            if (List<VT, Alloc>& nextBucket = buckets[i]; !nextBucket.empty()) {
                return Iterator(this, i, nextBucket.begin());
            }
        }
        return end();
    }
};

}

#endif // HASH_H
