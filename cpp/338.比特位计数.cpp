/*
 * @lc app=leetcode.cn id=338 lang=cpp
 *
 * [338] 比特位计数
 */

// @lc code=start
#include <vector>
using namespace std;
class Solution {
public:
    vector<int> countBits(int n) {
        vector<int> res(n + 1, 0);
        for (int i = 1; i < n + 1; ++i) {
            res[i] = res[i & (i - 1)] + 1;
        }
        return res;
    }
};
// @lc code=end

/*
常规暴力解法：
    vector<int> countBits(int n) {
        vector<int> res(n + 1, 0);
        for (int i = 0; i < n + 1; ++i) {
            int m = i;
            while (m) {
                res[i]++;
                m &= m - 1;
            }
        }
        return res;
    }
*/
