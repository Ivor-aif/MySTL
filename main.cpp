#include <iostream>

#include "headers/Array.h"
#include "headers/List.h"
#include "headers/Network.h"
#include "headers/String.h"

using namespace mySTL::containers;

int main() {
    const String str("Hello ");
    const String str2(std::string("World!").data());
    std::cout << str << str2 << std::endl;
    return 0;
}
