/*
 * @lc app=leetcode.cn id=3 lang=cpp
 *
 * [3] 无重复字符的最长子串
 */

// @lc code=start
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        if (s.empty()) return 0;
        int maxLen = 0;
        int left = 0;
        int count[128] = {0};
        for (int right = 0; right < s.size(); right++) {
            char c = s[right];
            count[c]++;
            while (count[c] > 1) {
                count[s[left]]--;
                left++;
            }
            maxLen = max(maxLen, right - left + 1);
        }
        return maxLen;
    }
};
// @lc code=end

