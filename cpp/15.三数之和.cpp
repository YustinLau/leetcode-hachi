/*
 * @lc app=leetcode.cn id=15 lang=cpp
 *
 * [15] 三数之和
 */

// @lc code=start
#include <vector>
#include <algorithm>
using namespace std;
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> res;
        int n = nums.size();
        for (int i = 0; i < n; i++) {
            if (nums[i] > 0) {break;}

            if (i > 0 && (nums[i] == nums[i - 1])) {continue;}
            
            int left = i + 1;
            int right = n - 1;
            while (left < right) {
                
                int sum = nums[left] + nums[right] + nums[i];
                if (sum == 0) {
                    res.push_back({nums[i], nums[left], nums[right]});
                    while (left < right && nums[left] == nums[left + 1]) {left++;}
                    while (left < right && nums[right] == nums[right - 1]) {right--;}
                    left++;
                    right--;
                } else if (sum > 0)
                {
                    right--;
                } else {
                    left++;
                } 
                
            }
        }
        return res;
    }
};
// @lc code=end

