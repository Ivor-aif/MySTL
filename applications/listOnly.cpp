#include "../headers/List.h"

// Welcome to submit any other usages as below!

/**
 * @author Ivor
 * @source https://leetcode.cn/problems/sort-list/description/
 * @date 2026-02-08, 13:20(UTC+8)
 * @consume 147ms, 100.70MB
 */
// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
ListNode* sortList(ListNode* head) {
    using namespace mySTL::containers;

    List<int> lst;
    ListNode* tmp = head;
    while (tmp) {
        lst.pushBack(tmp->val);
        tmp = tmp->next;
    }
    lst.sort();
    tmp = head;
    for(const auto& it : lst) {
        tmp->val = it;
        tmp = tmp->next;
    }
    return head;
}
