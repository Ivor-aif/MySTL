#include "../headers/Network.h"

#include <vector>
using namespace std;

// Welcome to submit any other usages as below!

/**
 * @author Ivor
 * @source https://leetcode.cn/problems/number-of-provinces/description/
 * @date 2026-02-10, 18:33(UTC+8)
 * @consume 0ms, 22.29MB
 */
void dfs(mySTL::containers::Network<int>::Site* site, std::vector<bool>& isVisited) {
    isVisited[site->getValue()] = true;
    for (const auto& nbr : site->getNeighbors()) {
        if (!isVisited[nbr->getValue()]) {
            dfs(nbr, isVisited);
        }
    }
}

int findCircleNum(vector<vector<int>>& isConnected) {
    using namespace mySTL::containers;

    Network<int> net;
    for (int i = 0; i < isConnected.size(); i++) {
        net.create(i);
    }
    for (int i = 0; i < isConnected.size(); i++) {
        for (int j = 0; j < isConnected[i].size(); j++) {
            if (i == j) {
                continue;
            }
            if (isConnected[i][j]) {
                net.connect(net.getSites()[i], net.getSites()[j]);
            }
        }
    }
    int ans = 0;
    std::vector<bool> isVisited(isConnected.size(), false);
    for (const auto& st : net.getSites()) {
        if (!isVisited[st->getValue()]) {
            ++ans;
            dfs(st, isVisited);
        }
    }
    return ans;
}
