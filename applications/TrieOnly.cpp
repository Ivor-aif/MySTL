#include <vector>
#include <algorithm>
#include <functional>

#include "../headers/Trie.h"
using namespace std;

// Welcome to submit any other usages as below!

/**
 * @author Ivor
 * @source https://leetcode.cn/problems/concatenated-words/description/
 * @date 2026-02-28, 15:14(UTC+8)
 * @consume 1123ms, 414.90MB
 */
vector<string> findAllConcatenatedWordsInADict(vector<string>& words) {
    using namespace mySTL::structures;

    sort(words.begin(), words.end(), [](const string& left, const string& right) {
            return left.size() < right.size();
        });
    vector<string> ans;
    Trie<char> trie;
    for (const auto& word : words) {
        size_t len = word.length();
        if (len == 0) {
            continue;
        }
        vector<int> memo(word.size(), -1);
        function<bool(int,int)> dfs = [&](int start, int count) -> bool {
            if (start == word.size()) {
                return count >= 2;
            }
            if (memo[start] != -1) {
                return memo[start];
            }
            for (int end = start + 1; end <= word.size(); ++end) {
                if (trie.contains(word.c_str() + start, end - start)) {
                    if (dfs(end, count + 1)) {
                        memo[start] = 1;
                        return true;
                    }
                }
            }
            memo[start] = 0;
            return false;
        };
        if (dfs(0, 0)) {
            ans.push_back(word);
        }
        trie.insert(word.c_str(), word.size());
    }
    return ans;
}
