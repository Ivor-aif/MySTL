#include <iostream>

#include "headers/Array.h"
#include "headers/List.h"
#include "headers/Network.h"
#include "headers/String.h"
#include "headers/Hash.h"

using namespace mySTL::containers;

int main() {
    Hash<int, bool> hash;
    hash[123] = true;
    hash[456] = true;
    hash[789] = true;
    if (hash.find(123) != hash.end()) {
        std::cout << hash[123] << std::endl;
    }
    return 0;
}
