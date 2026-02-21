#include <iostream>
#include <queue>

#include "headers/Array.h"
#include "headers/List.h"
#include "headers/Network.h"
#include "headers/String.h"
#include "headers/Hash.h"
#include "headers/Stack.h"
#include "headers/Queue.h"
#include "headers/Dequeue.h"

using namespace mySTL::containers;
using namespace mySTL::structures;

int main() {
    Dequeue<int> dq;
    dq.pushBack(10);
    dq.pushBack(20);
    dq.pushBack(30);
    dq.pushFront(40);
    dq.pushFront(50);
    std::cout << dq.popFront() << std::endl;
    while (!dq.empty()) {
        std::cout << dq.popBack() << std::endl;
    }
    return 0;
}
