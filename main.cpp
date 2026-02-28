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
#include "headers/Heap.h"

using namespace mySTL::containers;
using namespace mySTL::structures;

int main() {
    std::cout << "Testing Heap..." << std::endl;
    Heap<int> maxHeap;
    maxHeap.push(10);
    maxHeap.push(5);
    maxHeap.push(20);
    maxHeap.push(1);
    assert(maxHeap.top() == 20);
    maxHeap.pop();
    assert(maxHeap.top() == 10);
    maxHeap.pop();
    assert(maxHeap.top() == 5);
    assert(maxHeap.getSize() == 2);
    assert(!maxHeap.empty());
    maxHeap.pop();
    assert(maxHeap.top() == 1);
    maxHeap.pop();
    assert(maxHeap.empty());
    Heap<int, std::greater<>> minHeap;
    minHeap.push(10);
    minHeap.push(5);
    minHeap.push(20);
    assert(minHeap.top() == 5);
    minHeap.pop();
    assert(minHeap.top() == 10);
    std::cout << "Heap tests passed!" << std::endl;
    return 0;
}
