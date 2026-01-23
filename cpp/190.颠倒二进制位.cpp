/*
 * @lc app=leetcode.cn id=190 lang=cpp
 *
 * [190] 颠倒二进制位
 */

// @lc code=start
class Solution {
public:
    int reverseBits(int n) {
        // 1. 交换相邻的 16 位
        n = (n >> 16) | (n << 16);
        // 2. 交换相邻的 8 位
        n = ((n & 0xff00ff00) >> 8) | ((n & 0x00ff00ff) << 8);
        // 3. 交换相邻的 4 位
        n = ((n & 0xf0f0f0f0) >> 4) | ((n & 0x0f0f0f0f) << 4);
        // 4. 交换相邻的 2 位
        n = ((n & 0xcccccccc) >> 2) | ((n & 0x33333333) << 2);
        // 5. 交换相邻的 1 位
        n = ((n & 0xaaaaaaaa) >> 1) | ((n & 0x55555555) << 1);
        return n;
    }
};
// @lc code=end

/*
传统做法：按位与得到末尾数字后再让结果左移
        int res = 0;
        for (int i = 0; i < 32; i++) {
            res = (res << 1) | (n & 1);
            n >>= 1;
        }
        return res;
*/
