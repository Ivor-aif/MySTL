#include <vector>

#include "../headers/Stack.h"
using namespace std;

// Welcome to submit any other usages as below!

/**
 * @author Ivor
 * @source https://leetcode.cn/problems/trapping-rain-water/description/
 * @date 2026-02-17, 14:59(UTC+8)
 * @consume 0ms, 26.48MB
 */
int trap(vector<int>& height) {
    using namespace mySTL::structures;

    Stack<int> stack;
    int ans = 0;
    for (int i = 0; i < height.size(); i++) {
        int hei = height[i];
        while (!stack.empty() && height[stack.top()] < hei) {
            int lh = height[stack.top()];
            stack.pop();
            if (stack.empty()) {
                break;
            }
            int eh = min(height[stack.top()], hei);
            ans += (i - stack.top() - 1) * (eh - lh);
        }
        stack.push(i);
    }
    return ans;
}
