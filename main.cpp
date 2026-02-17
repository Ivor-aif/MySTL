#include <iostream>
#include <queue>

#include "headers/Array.h"
#include "headers/List.h"
#include "headers/Network.h"
#include "headers/String.h"
#include "headers/Hash.h"
#include "headers/Stack.h"
#include "headers/Queue.h"

using namespace mySTL::containers;
using namespace mySTL::structures;

int main() {
    Stack<int> stack;
    Queue<int> queue;
    queue.push(10);
    queue.push(20);
    queue.push(30);
    std::cout << queue.front() << "\t" << queue.back() << std::endl;
    queue.pop();
    return 0;
}
