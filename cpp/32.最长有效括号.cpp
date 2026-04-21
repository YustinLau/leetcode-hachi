/*
 * @lc app=leetcode.cn id=32 lang=cpp
 *
 * [32] 最长有效括号
 */

// @lc code=start
#include <stack>
using namespace std;
class Solution {
public:
    int longestValidParentheses(string s) {
        stack<int> st;//注意栈里存的是字符下标而不是值
        int maxLen = 0;
        st.push(-1);
        int size = s.size();
        for (int i = 0; i < size; i++) {
            if (s[i] == '(') {
                st.push(i);
            } else {
                st.pop();//默认-1弹出去无所谓
                if (st.empty()) {
                    st.push(i);//如果没有可以匹配的 说明当前）也没用 把下标弹进去
                } else {
                    maxLen = max(maxLen, i - st.top());
                    //如果里面有（值可以匹配，那么就用i减去下标算距离
                }

            }
        }
        return maxLen;
    }
};
// @lc code=end

