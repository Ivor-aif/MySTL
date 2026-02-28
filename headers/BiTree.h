//
// Created by Ivor_Aif on 2026/2/21.
//

#ifndef BITREE_H
#define BITREE_H

#include <cassert>

#include "Array.h"
#include "Queue.h"

namespace mySTL::structures {

template<typename T, typename Alloc = std::allocator<T>>
class BiTree {
public:
    struct Node {
        T value;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;

        template<typename... Args>
        explicit Node(Node* pat, Args&&... args) : value(std::forward<Args>(args)...), parent(pat) {

        }
            
        const T& getValue() const noexcept {
            return value;
        }
        
        T& getValue() noexcept {
            return value;
        }
    };

private:
    using NodeAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
    using NodeAllocTraits = std::allocator_traits<NodeAlloc>;
    using nodePtrAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Node*>;
    using LevelQueue = Queue<Node*, nodePtrAlloc>;

    NodeAlloc nodeAlloc;
    Node* root = nullptr;
    size_t nodeCount = 0;

    template<typename... Args>
    Node* createNode(Node* parent, Args&&... args) {
        Node* node = NodeAllocTraits::allocate(nodeAlloc, 1);
        try {
            NodeAllocTraits::construct(nodeAlloc, node, parent, std::forward<Args>(args)...);
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
    }

    void destroySubtree(Node* node) noexcept {
        if (!node) {
            return;
        }
        destroySubtree(node->left);
        destroySubtree(node->right);
        destroyNode(node);
    }
    
    void preOrderTraversal(Node* node, containers::Array<T>& out) const {
        if (!node) {
            return;
        }
        out.pushBack(node->value);
        preOrderTraversal(node->left, out);
        preOrderTraversal(node->right, out);
    }

    void inOrderTraversal(Node* node, containers::Array<T>& out) const {
        if (!node) {
            return;
        }
        inOrderTraversal(node->left, out);
        out.pushBack(node->value);
        inOrderTraversal(node->right, out);
    }

    void postOrderTraversal(Node* node, containers::Array<T>& out) const {
        if (!node) {
            return;
        }
        postOrderTraversal(node->left, out);
        postOrderTraversal(node->right, out);
        out.pushBack(node->value);
    }
    
    static Node* leftMost(Node* node) noexcept {
        if (!node) {
            return nullptr;
        }
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    size_t eraseSubtreeRecursive(Node* node) {
        if (!node) {
            return 0;
        }
        size_t count = 1;
        count += eraseSubtreeRecursive(node->left);
        count += eraseSubtreeRecursive(node->right);
        destroyNode(node);
        return count;
    }

public:
    class Iterator;
    class ConstIterator;

    BiTree() : BiTree(Alloc()) {

    }

    explicit BiTree(const Alloc& alloc) : nodeAlloc(alloc), root(nullptr), nodeCount(0) {

    }

    BiTree(const BiTree&) = delete;
    BiTree& operator=(const BiTree&) = delete;

    BiTree(BiTree&& other) noexcept : nodeAlloc(std::move(other.nodeAlloc)), root(other.root), nodeCount(other.nodeCount) {
        other.root = nullptr;
        other.nodeCount = 0;
    }

    BiTree& operator=(BiTree&& other) noexcept(NodeAllocTraits::propagate_on_container_move_assignment::value || NodeAllocTraits::is_always_equal::value) {
        if (this == &other) {
            return *this;
        }
        clear();
        if constexpr (NodeAllocTraits::propagate_on_container_move_assignment::value) {
            nodeAlloc = std::move(other.nodeAlloc);
        }
        root = other.root;
        nodeCount = other.nodeCount;
        other.root = nullptr;
        other.nodeCount = 0;
        return *this;
    }

    ~BiTree() {
        clear();
    }

    [[nodiscard]] bool empty() const noexcept {
        return nodeCount == 0;
    }

    [[nodiscard]] size_t getSize() const noexcept {
        return nodeCount;
    }

    void clear() {
        destroySubtree(root);
        root = nullptr;
        nodeCount = 0;
    }

    Node* getRoot() noexcept {
        return root;
    }
    
    const Node* getRoot() const noexcept {
        return root;
    }

    Node* getLeft(Node* node) const noexcept {
        return node ? node->left : nullptr;
    }
    
    const Node* getLeft(const Node* node) const noexcept {
        return node ? node->left : nullptr;
    }

    Node* getRight(Node* node) const noexcept {
        return node ? node->right : nullptr;
    }
    
    const Node* getRight(const Node* node) const noexcept {
        return node ? node->right : nullptr;
    }

    Node* getParent(Node* node) const noexcept {
        return node ? node->parent : nullptr;
    }
    
    const Node* getParent(const Node* node) const noexcept {
        return node ? node->parent : nullptr;
    }

    template<typename... Args>
    Node* insertRoot(Args&&... args) {
        assert(root == nullptr);
        Node* node = createNode(nullptr, std::forward<Args>(args)...);
        root = node;
        ++nodeCount;
        return node;
    }

    template<typename... Args>
    Node* insertLeft(Node* parent, Args&&... args) {
        assert(parent != nullptr);
        assert(parent->left == nullptr);
        Node* node = createNode(parent, std::forward<Args>(args)...);
        parent->left = node;
        ++nodeCount;
        return node;
    }

    template<typename... Args>
    Node* insertRight(Node* parent, Args&&... args) {
        assert(parent != nullptr);
        assert(parent->right == nullptr);
        Node* node = createNode(parent, std::forward<Args>(args)...);
        parent->right = node;
        ++nodeCount;
        return node;
    }

    void erase(Node* node) {
        if (!node) {
            return;
        }
        
        if (node->parent) {
            if (node->parent->left == node) {
                node->parent->left = nullptr;
            } else if (node->parent->right == node) {
                node->parent->right = nullptr;
            }
        } else if (node == root) {
            root = nullptr;
        }

        const size_t removed = eraseSubtreeRecursive(node);
        nodeCount -= removed;
    }

    containers::Array<T> preOrder() const {
        containers::Array<T> result;
        result.reserve(nodeCount);
        preOrderTraversal(root, result);
        return result;
    }

    containers::Array<T> inOrder() const {
        containers::Array<T> result;
        result.reserve(nodeCount);
        inOrderTraversal(root, result);
        return result;
    }

    containers::Array<T> postOrder() const {
        containers::Array<T> result;
        result.reserve(nodeCount);
        postOrderTraversal(root, result);
        return result;
    }

    containers::Array<T> levelOrder() const {
        containers::Array<T> result;
        if (!root) {
            return result;
        }
        result.reserve(nodeCount);
        LevelQueue queue;
        queue.push(root);
        while (!queue.empty()) {
            Node* curr = queue.front();
            queue.pop();
            result.pushBack(curr->value);
            if (curr->left) {
                queue.push(curr->left);
            }
            if (curr->right) {
                queue.push(curr->right);
            }
        }
        return result;
    }

    Iterator begin() noexcept {
        return Iterator(leftMost(root));
    }

    Iterator end() noexcept {
        return Iterator(nullptr);
    }

    ConstIterator begin() const noexcept {
        return cBegin();
    }

    ConstIterator end() const noexcept {
        return cEnd();
    }

    ConstIterator cBegin() const noexcept {
        return ConstIterator(leftMost(root));
    }

    ConstIterator cEnd() const noexcept {
        return ConstIterator(nullptr);
    }

    class Iterator {
        friend class BiTree;

    private:
        Node* current;
        
        explicit Iterator(Node* node) : current(node) {

        }

        void increment() {
            if (!current) {
                return;
            }
            
            if (current->right) {
                current = current->right;
                while (current->left) {
                    current = current->left;
                }
            } else {
                Node* pat = current->parent;
                while (pat && current == pat->right) {
                    current = pat;
                    pat = pat->parent;
                }
                current = pat;
            }
        }

    public:
        Iterator() : current(nullptr) {

        }

        T& operator*() const {
            return current->value;
        }

        T* operator->() const {
            return &current->value;
        }

        Iterator& operator++() {
            increment();
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            increment();
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    class ConstIterator {
        friend class BiTree;

    private:
        const Node* current;

        explicit ConstIterator(const Node* node) : current(node) {

        }

        void increment() {
            if (!current) {
                return;
            }
            if (current->right) {
                current = current->right;
                while (current->left) {
                    current = current->left;
                }
            } else {
                const Node* pat = current->parent;
                while (pat && current == pat->right) {
                    current = pat;
                    pat = pat->parent;
                }
                current = pat;
            }
        }

    public:
        ConstIterator() : current(nullptr) {

        }

        explicit ConstIterator(const Iterator& it) : current(it.current) {

        }

        const T& operator*() const {
            return current->value;
        }

        const T* operator->() const {
            return &current->value;
        }

        ConstIterator& operator++() {
            increment();
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator tmp = *this;
            increment();
            return tmp;
        }

        bool operator==(const ConstIterator& other) const {
            return current == other.current;
        }

        bool operator!=(const ConstIterator& other) const {
            return current != other.current;
        }
    };
};

}

#endif // BITREE_H
