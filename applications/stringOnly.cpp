#include "../headers/String.h"
using namespace std;

// Welcome to submit any other usages as below!

/**
 * @author Ivor
 * @source https://leetcode.cn/problems/substrings-of-size-three-with-distinct-characters/description/
 * @date 2026-02-14, 19:18(UTC+8)
 * @consume 0ms, 8.63MB
 */
int countGoodSubstrings(string s) {
    using namespace mySTL::containers;

    String str(s.data());
    if (str.length() < 3) {
        return 0;
    }
    int count = 0;
    for (size_t i = 0; i + 2 < str.length(); i++) {
        count += (str[i] != str[i+1] && str[i] != str[i+2] && str[i+1] != str[i+2]);
    }
    return count;
}
