#include <vector>

#include "../headers/Hash.h"
using namespace std;

// Welcome to submit any other usages as below!

/**
 * @author Ivor
 * @source https://leetcode.cn/problems/two-sum/description/
 * @date 2026-02-16, 17:30(UTC+8)
 * @consume 11ms, 18.98MB
 */
vector<int> twoSum(vector<int>& nums, int target) {
    using namespace mySTL::containers;

    Hash<int, int> hash;
    int index = 0;
    for (const auto& num : nums) {
        if (hash.find(target - num) != hash.end()) {
            return vector{hash.find(target - num)->second, index};
        } else {
            hash[num] = index;
            index++;
        }
    }
    return vector(0, 0);
}
