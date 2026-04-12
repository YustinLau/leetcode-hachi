/*
 * @lc app=leetcode.cn id=875 lang=cpp
 *
 * [875] 爱吃香蕉的珂珂
 */

// @lc code=start
#include <algorithm>
class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int left = 1;
        int right = 0;
        for (int p : piles) right = max(right, p);
        right++; // [left, right)

        while (left < right) {
            int mid = left + (right - left) / 2;
            if (canEatAll(piles, mid, h)) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        return left;
    }
private:
    bool canEatAll(vector<int>& piles, int k, int h) {
        long hours = 0;
        for (auto pile : piles) {
            hours += (pile + k - 1) / k;
        }
        return hours <= h;
    }
};
// @lc code=end

