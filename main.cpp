#include <iostream>

#include "headers/Array.h"
#include "headers/List.h"

using namespace mySTL::containers;

int main() {
    List<int> lst;
    lst.pushBack(1);
    lst.pushBack(5);
    lst.pushBack(3);
    lst.sort();
    for (const int& it : lst) {
        std::cout << it << ' ';
    }
    return 0;
}
