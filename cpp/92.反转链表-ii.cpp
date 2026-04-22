/*
 * @lc app=leetcode.cn id=92 lang=cpp
 *
 * [92] 反转链表 II
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        if (!head || left == right) return head;
        ListNode* dummy = new ListNode(0); // 注意这里dummy初始化要new
        dummy->next = head;
        ListNode* prev = dummy;
        for (int i = 0; i < left - 1; i++) {
            prev = prev->next; // 注意这里还不需要有curr 只是移动节点就可以
        }
        ListNode* curr = prev->next; // 这里才需要引入curr
        for (int i = 0; i < right - left; i++) {
            ListNode* tmp = curr->next;
            curr->next = tmp->next;
            tmp->next = prev->next;
            prev->next = tmp;
        }
        return dummy->next;
    }
};
// @lc code=end

