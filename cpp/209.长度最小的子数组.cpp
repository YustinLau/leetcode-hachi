/*
 * @lc app=leetcode.cn id=209 lang=cpp
 *
 * [209] 长度最小的子数组
 */

// @lc code=start
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int left = 0;
        int n = nums.size();
        int minLen = __INT_MAX__;
        int sum = 0;
        for (int right = 0; right < n; right++) { // using for not regular while can reduce o(n^2)
            sum += nums[right];
            while (sum >= target) { // while not if for jumping duplicate elements
                minLen = min(right - left + 1, minLen);
                sum -= nums[left]; // don't forget to erompt left
                left++;
            }
        }
        return (minLen == __INT_MAX__) ? 0 : minLen;
    }
};
// @lc code=end

