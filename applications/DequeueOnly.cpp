#include <vector>

#include "../headers/Dequeue.h"
using namespace std;

// Welcome to submit any other usages as below!

/**
 * @author Ivor
 * @source https://leetcode.cn/problems/longest-continuous-subarray-with-absolute-diff-less-than-or-equal-to-limit/description/
 * @date 2026-02-21, 16:50(UTC+8)
 * @consume 264ms, 140.24MB
 */
int longestSubarray(vector<int>& nums, int limit) {
    using namespace mySTL::structures;

    Dequeue<int> qMax, qMin;
    int left = 0, right = 0, ans = 0;
    while (right < nums.size()) {
        while (!qMax.empty() && qMax.front() < nums[right]) {
            qMax.popFront();
        }
        qMax.pushFront(nums[right]);
        while (!qMin.empty() && qMin.front() > nums[right]) {
            qMin.popFront();
        }
        qMin.pushFront(nums[right]);
        while (!qMax.empty() && !qMin.empty() && qMax.back() - qMin.back() > limit) {
            if (qMin.back() == nums[left]) {
                qMin.popBack();
            }
            if (qMax.back() == nums[left]) {
                qMax.popBack();
            }
            ++left;
        }
        int len = right - left + 1;
        ans = max(ans, len);
        ++right;
    }
    return ans;
}
