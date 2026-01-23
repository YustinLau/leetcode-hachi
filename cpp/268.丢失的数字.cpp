/*
 * @lc app=leetcode.cn id=268 lang=cpp
 *
 * [268] 丢失的数字
 */

// @lc code=start
#include <vector>
using namespace std;
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int res = nums.size();
        for (int i = 0; i < nums.size(); ++i) {
            res ^= i ^ nums[i];
        }
        return res;
    }
};
// @lc code=end
/*
另一种解法根据高斯求和公式，计算0到n的和减去数组元素和即为缺失数字。
但这个方法可能会有整数溢出的问题，尤其是在n较大的情况下。
*/
