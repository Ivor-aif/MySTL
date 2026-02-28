//
// Created by Ivor_Aif on 2026/2/25.
//

#ifndef TRIE_H
#define TRIE_H

#include "Array.h"

namespace mySTL::structures {

template<typename T, typename Alloc = std::allocator<T>>
class Trie {
public:
    struct Node {
        bool isEnd = false;
        Node* parent = nullptr;
        containers::Array<T> keys;
        containers::Array<Node*> children;

        explicit Node(Node* pat) : parent(pat) {

        }

        containers::detail::KeyValue<size_t, bool> findKey(const T& key) const {
            if (keys.empty()) {
                return {0, false};
            }
            auto it = std::lower_bound(keys.begin(), keys.end(), key);
            size_t idx = std::distance(keys.begin(), it);
            if (it != keys.end() && *it == key) {
                return {idx, true};
            }
            return {idx, false};
        }
    };

private:
    using NodeAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
    using NodeAllocTraits = std::allocator_traits<NodeAlloc>;

    NodeAlloc nodeAlloc;
    Node* root = nullptr;
    size_t nodeCount = 0;
    size_t wordCount = 0;

    template<typename... Args>
    Node* createNode(Node* parent, Args&&... args) {
        Node* node = NodeAllocTraits::allocate(nodeAlloc, 1);
        try {
            NodeAllocTraits::construct(nodeAlloc, node, parent, std::forward<Args>(args)...);
            nodeCount++;
        } catch (...) {
            NodeAllocTraits::deallocate(nodeAlloc, node, 1);
            throw;
        }
        return node;
    }

    void destroyNode(Node* node) noexcept {
        if (!node) {
            return;
        }
        NodeAllocTraits::destroy(nodeAlloc, node);
        NodeAllocTraits::deallocate(nodeAlloc, node, 1);
        if (nodeCount > 0) {
            nodeCount--;
        }
    }

    void destroySubtree(Node* node) noexcept {
        if (!node) {
            return;
        }
        for (auto& child : node->children) {
            destroySubtree(child);
        }
        destroyNode(node);
    }

    Node* cloneNode(Node* src, Node* parent) {
        if (!src) {
            return nullptr;
        }
        Node* newNode = createNode(parent);
        newNode->isEnd = src->isEnd;
        newNode->keys = src->keys;
        for (size_t i = 0; i < src->children.getSize(); ++i) {
            newNode->children.pushBack(cloneNode(src->children[i], newNode));
        }
        return newNode;
    }

    void insertChild(Node* parent, size_t index, T key, Node* child) {
        parent->keys.pushBack(key);
        parent->children.pushBack(nullptr);
        if (index < parent->keys.getSize() - 1) {
            for (size_t i = parent->keys.getSize() - 1; i > index; --i) {
                parent->keys[i] = parent->keys[i-1];
                parent->children[i] = parent->children[i-1];
            }
        }
        
        parent->keys[index] = key;
        parent->children[index] = child;
    }
    
    void removeChild(Node* parent, size_t index) {
        if (index >= parent->keys.getSize()) {
            return;
        }
        for (size_t i = index; i < parent->keys.getSize() - 1; ++i) {
            parent->keys[i] = parent->keys[i+1];
            parent->children[i] = parent->children[i+1];
        }
        parent->keys.popBack();
        parent->children.popBack();
    }

public:
    class ConstIterator;

    class Iterator {
        friend class Trie;
        friend class ConstIterator;
        Node* current;
        containers::Array<T> currentWord;
        Trie* trie;

    public:
        Iterator() : current(nullptr), trie(nullptr) {

        }

        Iterator(Node* node, containers::Array<T> word, Trie* trie) : current(node), currentWord(std::move(word)), trie(trie) {

        }

        const containers::Array<T>& operator*() const {
            return currentWord;
        }

        const containers::Array<T>* operator->() const {
            return &currentWord;
        }

        Iterator& operator++() {
            if (!current) {
                return *this;
            }
            if (!current->children.empty()) {
                current = current->children[0];
                currentWord.pushBack(current->parent->keys[0]);
                if (current->isEnd) {
                    return *this;
                }
                return operator++();
            }
            while (current->parent) {
                auto& parentChildren = current->parent->children;
                size_t idx = 0;
                bool found = false;
                for (size_t i = 0; i < parentChildren.getSize(); ++i) {
                    if (parentChildren[i] == current) {
                        idx = i;
                        found = true;
                        break;
                    }
                }
                if (found && idx + 1 < parentChildren.getSize()) {
                    current = parentChildren[idx + 1];
                    currentWord.popBack();
                    currentWord.pushBack(current->parent->keys[idx + 1]);
                    if (current->isEnd) {
                        return *this;
                    }
                    return operator++();
                }
                current = current->parent;
                if (!currentWord.empty()) {
                    currentWord.popBack();
                }
            }
            current = nullptr;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++*this;
            return temp;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
        
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        bool operator==(const ConstIterator& other) const;
        bool operator!=(const ConstIterator& other) const;
    };

    class ConstIterator {
        friend class Trie;
        friend class Iterator;
        const Node* current;
        containers::Array<T> currentWord;
        const Trie* trie;

    public:
        ConstIterator() : current(nullptr), trie(nullptr) {

        }

        ConstIterator(const Node* node, containers::Array<T> word, const Trie* trie) : current(node), currentWord(std::move(word)), trie(trie) {
            
        }

        ConstIterator(const Iterator& other) : current(other.current), currentWord(other.currentWord), trie(other.trie) {
            
        }

        const containers::Array<T>& operator*() const {
            return currentWord;
        }

        const containers::Array<T>* operator->() const {
            return &currentWord;
        }

        ConstIterator& operator++() {
            if (!current) {
                return *this;
            }
            if (!current->children.empty()) {
                current = current->children[0];
                currentWord.pushBack(current->parent->keys[0]);
                if (current->isEnd) {
                    return *this;
                }
                return operator++();
            }
            while (current->parent) {
                auto& parentChildren = current->parent->children;
                size_t idx = 0;
                bool found = false;
                for (size_t i = 0; i < parentChildren.getSize(); ++i) {
                    if (parentChildren[i] == current) {
                        idx = i;
                        found = true;
                        break;
                    }
                }
                if (found && idx + 1 < parentChildren.getSize()) {
                    current = parentChildren[idx + 1];
                    currentWord.popBack();
                    currentWord.pushBack(current->parent->keys[idx + 1]);
                    if (current->isEnd) {
                        return *this;
                    }
                    return operator++();
                }
                current = current->parent;
                if (!currentWord.empty()) {
                    currentWord.popBack();
                }
            }
            current = nullptr;
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator temp = *this;
            ++*this;
            return temp;
        }

        bool operator==(const ConstIterator& other) const {
            return current == other.current;
        }

        bool operator!=(const ConstIterator& other) const {
            return !(*this == other);
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    Trie() : nodeAlloc(Alloc()), root(nullptr), nodeCount(0), wordCount(0) {
        root = createNode(nullptr);
    }

    explicit Trie(const Alloc& alloc) : nodeAlloc(alloc), root(nullptr), nodeCount(0), wordCount(0) {
        root = createNode(nullptr);
    }

    ~Trie() {
        if (root) {
            destroySubtree(root);
        }
    }

    Trie(const Trie& other) : nodeAlloc(other.nodeAlloc), root(nullptr), nodeCount(0), wordCount(other.wordCount) {
        if (other.root) {
            root = cloneNode(other.root, nullptr);
            nodeCount = other.nodeCount;
        } else {
            root = createNode(nullptr);
            nodeCount = 1;
        }
    }

    Trie& operator=(const Trie& other) {
        if (this == &other) {
            return *this;
        }
        if (root) {
            destroySubtree(root);
        }
        nodeAlloc = other.nodeAlloc;
        wordCount = other.wordCount;
        if (other.root) {
            root = cloneNode(other.root, nullptr);
            nodeCount = other.nodeCount;
        } else {
            root = createNode(nullptr);
            nodeCount = 1;
        }
        return *this;
    }

    Trie(Trie&& other) noexcept : nodeAlloc(std::move(other.nodeAlloc)), root(other.root), nodeCount(other.nodeCount), wordCount(other.wordCount) {
        other.root = nullptr;
        other.nodeCount = 0;
        other.wordCount = 0;
    }

    Trie& operator=(Trie&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        if (root) {
            destroySubtree(root);
        }
        nodeAlloc = std::move(other.nodeAlloc);
        root = other.root;
        nodeCount = other.nodeCount;
        wordCount = other.wordCount;
        other.root = nullptr;
        other.nodeCount = 0;
        other.wordCount = 0;
        return *this;
    }

    void clear() {
        if (!root) {
            return;
        }
        for (auto& child : root->children) {
            destroySubtree(child);
        }
        root->keys.clear();
        root->children.clear();
        root->isEnd = false;
        nodeCount = 1;
        wordCount = 0;
    }

    void insert(const containers::Array<T>& word) {
        insert(word.getData(), word.getSize());
    }

    void insert(const T* str, const size_t len) {
        if (!root) {
            root = createNode(nullptr);
        }
        Node* current = root;
        for (size_t i = 0; i < len; ++i) {
            T ch = str[i];
            auto [idx, found] = current->findKey(ch);
            if (found) {
                current = current->children[idx];
            } else {
                Node* newNode = createNode(current);
                insertChild(current, idx, ch, newNode);
                current = newNode;
            }
        }
        if (!current->isEnd) {
            current->isEnd = true;
            wordCount++;
        }
    }

    bool contains(const containers::Array<T>& word) const {
        return contains(word.getData(), word.getSize());
    }

    bool contains(const T* str, const size_t len) const {
        if (!root) {
            return false;
        }
        Node* current = root;
        for (size_t i = 0; i < len; ++i) {
            T ch = str[i];
            auto [idx, found] = current->findKey(ch);
            if (!found) {
                return false;
            }
            current = current->children[idx];
        }
        return current->isEnd;
    }

    bool startsWith(const containers::Array<T>& prefix) const {
        return startsWith(prefix.getData(), prefix.getSize());
    }

    bool startsWith(const T* str, const size_t len) const {
        if (!root) {
            return false;
        }
        Node* current = root;
        for (size_t i = 0; i < len; ++i) {
            T ch = str[i];
            auto [idx, found] = current->findKey(ch);
            if (!found) {
                return false;
            }
            current = current->children[idx];
        }
        return true;
    }

    bool erase(const containers::Array<T>& word) {
        return erase(word.getData(), word.getSize());
    }

    bool erase(const T* str, const size_t len) {
        if (!root) {
            return false;
        }
        Node* current = root;
        for (size_t i = 0; i < len; ++i) {
            T ch = str[i];
            auto [idx, found] = current->findKey(ch);
            if (!found) {
                return false;
            }
            current = current->children[idx];
        }
        
        if (!current->isEnd) {
            return false;
        }
        current->isEnd = false;
        wordCount--;
        while (current != root && !current->isEnd && current->children.empty()) {
            Node* parent = current->parent;
            size_t idx = 0;
            bool found = false;
            for(size_t i=0; i<parent->children.getSize(); ++i) {
                if (parent->children[i] == current) {
                    idx = i;
                    found = true;
                    break;
                }
            }
            if (found) {
                destroyNode(current);
                removeChild(parent, idx);
                current = parent;
            } else {
                break;
            }
        }
        return true;
    }

    [[nodiscard]] size_t size() const noexcept {
        return wordCount;
    }

    [[nodiscard]] size_t countNodes() const noexcept {
        return nodeCount;
    }

    [[nodiscard]] bool empty() const noexcept {
        return wordCount == 0;
    }

    Iterator begin() {
        if (!root) {
            return end();
        }
        Node* current = root;
        containers::Array<T> word;
        while (current) {
            if (current->isEnd) {
                return Iterator(current, word, this);
            }
            if (!current->children.empty()) {
                word.pushBack(current->keys[0]);
                current = current->children[0];
            } else {
                return end();
            }
        }
        return end();
    }

    Iterator end() {
        return Iterator(nullptr, containers::Array<T>(), this);
    }
    
    ConstIterator begin() const {
        if (!root) {
            return end();
        }
        const Node* current = root;
        containers::Array<T> word;
        while (current) {
            if (current->isEnd) {
                return ConstIterator(current, word, this);
            }            
            if (!current->children.empty()) {
                word.pushBack(current->keys[0]);
                current = current->children[0];
            } else {
                return end();
            }
        }
        return end();
    }

    ConstIterator end() const {
        return ConstIterator(nullptr, containers::Array<T>(), this);
    }
};

template<typename T, typename Alloc>
bool Trie<T, Alloc>::Iterator::operator==(const typename Trie<T, Alloc>::ConstIterator& other) const {
    return current == other.current;
}

template<typename T, typename Alloc>
bool Trie<T, Alloc>::Iterator::operator!=(const typename Trie<T, Alloc>::ConstIterator& other) const {
    return !(*this == other);
}

}

#endif // TRIE_H
