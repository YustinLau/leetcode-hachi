/*
 * @lc app=leetcode.cn id=34 lang=cpp
 *
 * [34] 在排序数组中查找元素的第一个和最后一个位置
 */

// @lc code=start
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        if (nums.empty()) return {-1, -1};
        int first = findBound(nums, target, true);
        if (first == -1) return {-1, -1};
        int second = findBound(nums, target, false);
        return {first, second};
    }
private:
    int findBound(vector<int>& nums, int target, bool isFirst) {
        int left = 0, right = nums.size();
        int ans = -1;
        while (left < right) {
            int mid = left + (right - left)/2;
            if (nums[mid] == target) {
                ans = mid;
                if (isFirst) {
                    right = mid;
                } else {
                    left = mid + 1;
                }
            } else if (nums[mid] < target){
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        return ans;
    }
};
// @lc code=end

