#include <iostream>

#include "headers/Array.h"

using namespace mySTL::containers;

int main() {
    Array<int> arr = {1, 2, 3};
    arr.get(1) = 42;
    std::cout << "Hello!" << std::endl;
    for (const int& xx : arr) {
        std::cout << xx << " ";
    }
    return 0;
}
