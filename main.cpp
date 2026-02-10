#include <iostream>

#include "headers/Array.h"
#include "headers/List.h"
#include "headers/Network.h"

using namespace mySTL::containers;

int main() {
    Network<int> network;
    for (int i = 0; i < 10; i++) {
        network.create(i);
    }
    for (const auto& st : network.getSites()) {
        std::cout << st->getValue() << std::endl;
    }
    return 0;
}
