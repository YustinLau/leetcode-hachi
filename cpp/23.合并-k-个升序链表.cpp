/*
 * @lc app=leetcode.cn id=23 lang=cpp
 *
 * [23] 合并 K 个升序链表
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
    struct compare {
        bool operator()(ListNode* l1, ListNode* l2) {
            return l1->val > l2->val; // 小顶堆：值越大的优先级越低（排在下面）
        }
    };
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        // 1. 定义小顶堆
        priority_queue<ListNode*, vector<ListNode*>, compare> pq;

        // 2. 将每个非空链表的头节点放入堆
        for (auto list : lists) {
            if (list) pq.push(list);
        }

        // 3. 经典的 Dummy 虚拟头节点
        ListNode* dummy = new ListNode(0);
        ListNode* tail = dummy;

        while (!pq.empty()) {
            // 取出最小的节点
            ListNode* minNode = pq.top();
            pq.pop();

            // 接入结果链表
            tail->next = minNode;
            tail = tail->next;

            // 如果这个节点后面还有人，就把接班人送进堆
            if (minNode->next) {
                pq.push(minNode->next);
            }
        }

        return dummy->next;
    }
};
// @lc code=end

