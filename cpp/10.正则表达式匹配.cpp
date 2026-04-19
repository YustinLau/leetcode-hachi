
/*
 * @lc app=leetcode.cn id=10 lang=cpp
 *
 * [10] 正则表达式匹配
 */

// @lc code=start
class Solution {
public:
    bool isMatch(string s, string p) {
        int m = s.size(), n = p.size();
        // dp[i][j] 代表 s的前i个字符和p的前j个字符是否匹配
        vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
        // 注意这里的写法，第一个是长度，第二个是初始化值，因为是二维嵌套，所以用vector初始化而不是两个长度入参

        // 1. 初始化
        dp[0][0] = true; 
        
        // 特殊处理：s为空，p不为空的情况（例如 s="", p="a*b*" 是匹配的）
        for (int j = 2; j <= n; j++) {
            if (p[j-1] == '*') {
                dp[0][j] = dp[0][j-2];
            }
        }

        // 2. 开始填表
        for (int i = 1; i <= m; i++) { //注意这里<=有等号，因为m+1
            for (int j = 1; j <= n; j++) {
                if (p[j-1] != '*') {
                    // 普通匹配或 '.' 匹配
                    if (s[i-1] == p[j-1] || p[j-1] == '.') {
                        dp[i][j] = dp[i-1][j-1];
                    }
                } else {
                    // 遇到 '*'，分两种大情况
                    // 情况1：匹配0次，直接看去掉 'x*' 之后的情况
                    dp[i][j] = dp[i][j-2];//理解每一种情况对应的场景再写表达式
                    
                    // 情况2：匹配1次或多次，前提是当前字符得对得上
                    if (s[i-1] == p[j-2] || p[j-2] == '.') {
                        // 只要匹配0次或多次有一种成功，dp[i][j] 就是 true
                        dp[i][j] = dp[i][j] || dp[i-1][j];//这里有个或
                    }
                }
            }
        }
        return dp[m][n];
    }
};
// @lc code=end

