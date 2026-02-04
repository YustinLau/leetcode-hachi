/*
 * @lc app=leetcode.cn id=167 lang=cpp
 *
 * [167] 两数之和 II - 输入有序数组
 */

// @lc code=start
#include <vector>
using namespace std;
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        vector<int> result;
        int left = 0;
        int right = numbers.size() - 1;
        while (left < right) {
            int sum = numbers[left] + numbers[right];
            if (sum == target) {
                result.push_back(left + 1);
                result.push_back(right + 1);
                break;
            } else if (sum < target) {
                left++;
            } else {
                right--;
            }
        } 
        return result;
    }
};
// @lc code=end

