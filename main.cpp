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

using namespace mySTL::containers;
using namespace mySTL::structures;

int main() {
    BiTree<int> bt;
    BiTree<int>::Node* root = bt.insertRoot(1);
    BiTree<int>::Node* left = bt.insertLeft(root, 2);
    Array<int> po = bt.preOrder();
    return 0;
}
