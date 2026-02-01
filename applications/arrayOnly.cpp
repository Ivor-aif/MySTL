#include "../headers/Array.h"

#include <vector>
using namespace std;

// Welcome to submit any other usages as below!

/**
 * @author Ivor
 * @source https://leetcode.cn/problems/kids-with-the-greatest-number-of-candies/description/
 * @date 2026-02-01, 12:38(UTC+8)
 * @consume 0ms, 12.45MB
 */
vector<bool> kidsWithCandies(vector<int>& candies, int extraCandies) {
    using namespace mySTL::containers;

    int maxC = 0;
    for (const int& candie: candies) {
        maxC = maxC > candie ? maxC : candie;
    }
    Array<bool> ret;
    for (const int& candie: candies) {
        ret.pushBack(candie + extraCandies >= maxC);
    }
    return vector(ret.begin(), ret.end());
}
