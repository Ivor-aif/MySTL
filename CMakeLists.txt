cmake_minimum_required(VERSION 3.31)
project(MySTL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(MySTL main.cpp
        headers/containers.h
        headers/array.h
        applications/arrayOnly.cpp
        headers/List.h
        applications/listOnly.cpp
        headers/Network.h
        applications/networkOnly.cpp
        headers/String.h
        applications/stringOnly.cpp
        headers/Hash.h
        applications/hashOnly.cpp
        headers/Stack.h
        applications/StackOnly.cpp
        headers/Queue.h
        applications/QueueOnly.cpp
        headers/Dequeue.h
        applications/dequeueOnly.cpp
        headers/BiTree.h
        applications/BiTreeOnly.cpp
        headers/Trie.h
        applications/TrieOnly.cpp
        headers/Heap.h
        applications/HeapOnly.cpp
)
