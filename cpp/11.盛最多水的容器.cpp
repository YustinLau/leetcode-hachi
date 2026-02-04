/*
 * @lc app=leetcode.cn id=11 lang=cpp
 *
 * [11] 盛最多水的容器
 */

// @lc code=start
#include <vector>
using namespace std;
class Solution {
public:
    int maxArea(vector<int>& height) {
        int max_water = 0;
        int left = 0;
        int right = height.size() - 1;
        while (left < right) {
            int cur_max = (right - left) * min(height[left], height[right]);
            max_water = max(cur_max, max_water);
            if (height[left] <= height[right]) {
                left++;
            } else {right--;}
        }
        return max_water;
    }
};
// @lc code=end

