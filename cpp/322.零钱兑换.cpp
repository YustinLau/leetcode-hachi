/*
 * @lc app=leetcode.cn id=322 lang=cpp
 *
 * [322] 零钱兑换
 */

// @lc code=start
#include <vector>
using namespace std;
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, amount + 1);  // amount + 1是为了没找出来的初始化判断
        dp[0] = 0;
        for (int i = 1; i <= amount; i++) {
            for (auto coin : coins) {
                if (i - coin >= 0) {// 目标余额大于币种面额时才需要考虑是否需要
                    dp[i] = min(dp[i - coin] + 1, dp[i]);// 选或不选
                }
            }
        }
        return dp[amount] > amount ? -1 : dp[amount]; // dp stands feor the leastes nums for amount
    }
};
// @lc code=end

