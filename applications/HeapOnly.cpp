#include <vector>

#include "../headers/Heap.h"
using namespace std;

// Welcome to submit any other usages as below!

/**
 * @author Ivor
 * @source https://leetcode.cn/problems/kth-largest-element-in-an-array/description/
 * @date 2026-02-28, 18:29(UTC+8)
 * @consume 55ms, 63.16MB
 */
int findKthLargest(vector<int>& nums, int k) {
    using namespace mySTL::structures;

    Heap<int> heap;
    for (const auto& num : nums) {
        heap.push(num);
    }
    for (int i = 0; i < k-1; i++) {
        heap.pop();
    }
    return heap.top();
}
