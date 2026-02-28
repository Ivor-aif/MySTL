#include <iostream>

#include "headers/Array.h"
#include "headers/List.h"
#include "headers/Network.h"
#include "headers/String.h"
#include "headers/Hash.h"
#include "headers/Stack.h"
#include "headers/Queue.h"
#include "headers/Dequeue.h"
#include "headers/BiTree.h"
#include "headers/Trie.h"

using namespace mySTL::containers;
using namespace mySTL::structures;

int main() {
    std::cout << "Testing Trie..." << std::endl;
    Trie<char> trie;
    trie.insert("apple", 5);
    trie.insert("app", 3);
    trie.insert("apricot", 7);
    assert(trie.contains("apple", 5));
    assert(trie.contains("app", 3));
    assert(trie.contains("apricot", 7));
    assert(!trie.contains("ap", 2));
    assert(trie.startsWith("ap", 2));
    assert(trie.startsWith("app", 3));
    assert(!trie.startsWith("ban", 3));
    std::cout << "Traversal:" << std::endl;
    for (const auto& word : trie) {
        for (size_t i = 0; i < word.getSize(); ++i) {
            std::cout << word[i];
        }
        std::cout << " ";
    }
    std::cout << std::endl;
    std::cout << "Const Iterator Test:" << std::endl;
    const Trie<char>& constTrie = trie;
    for (Trie<char>::ConstIterator it = constTrie.begin(); it != constTrie.end(); ++it) {
        for (size_t i = 0; i < it->getSize(); ++i) {
            std::cout << (*it)[i];
        }
        std::cout << " ";
    }
    std::cout << std::endl;
    const Trie<char>::Iterator it = trie.begin();
    const Trie<char>::ConstIterator cit = it;
    assert(it == cit);
    trie.erase("app", 3);
    assert(!trie.contains("app", 3));
    assert(trie.contains("apple", 5));
    std::cout << "Trie tests passed!" << std::endl;
    return 0;
}
