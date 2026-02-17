#include <vector>

#include "../headers/Queue.h"
using namespace std;

// Welcome to submit any other usages as below!

/**
 * @author Ivor
 * @source https://leetcode.cn/problems/time-needed-to-buy-tickets/description/
 * @date 2026-02-17, 15:37(UTC+8)
 * @consume 19ms, 10.33MB
 */
int timeRequiredToBuy(vector<int>& tickets, int k) {
    using namespace mySTL::structures;

    Queue<mySTL::containers::detail::KeyValue<int, int>> queue;
    for (int i = 0; i < tickets.size(); i++) {
        queue.push({i, tickets[i]});
    }
    int ans = 0;
    while (true) {
        auto info = queue.front();
        int idx = info.first;
        int remain = info.second;
        queue.pop();
        ans++;
        if (--remain > 0) {
            queue.push({idx, remain});
        } else if (idx == k) {
            break;
        }
    }
    return ans;
}
