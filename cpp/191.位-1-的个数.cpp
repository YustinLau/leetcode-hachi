/*
 * @lc app=leetcode.cn id=191 lang=cpp
 *
 * [191] 位1的个数
 */

// @lc code=start
class Solution {
public:
    int hammingWeight(int n) {
        int count = 0;
        while (n != 0) {
            n = n & (n - 1);
            count++;
        }
        return count;
    }
};
// @lc code=end

// 2026-01-22
// 逐位检查法
/*
    int hammingWeight(int n) {
        int count = 0;
        while (n != 0) {
            if (n & 1 == 1) {
                count++;
            }
            n = n >> 1;
        }
        return count;
    }
*/
