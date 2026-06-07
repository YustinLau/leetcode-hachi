/*
 * @lc app=leetcode.cn id=4 lang=cpp
 *
 * [4] 寻找两个正序数组的中位数
 */

// @lc code=start
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        if (nums1.size() > nums2.size()) {
            return findMedianSortedArrays(nums2, nums1);
        }
        int m = nums1.size();
        int n = nums2.size();
        int halfLen = (m + n + 1) / 2;
        int left = 0;
        int right = m;
        while (left <= right) {
            int i = left + (right - left) / 2;
            int j = halfLen - i;
            if (i > 0 && nums1[i - 1] > nums2[j]) {
                right = i  - 1;
            } else if (i < m && nums2[j - 1] > nums1[i]) {
                left = i + 1;
            } else {
                int leftMax = 0;
                if (i == 0) leftMax = nums2[j - 1];
                else if (j == 0) leftMax = nums1[i - 1];
                else {
                    leftMax = max(nums1[i - 1], nums2[j - 1]);
                }
                if ((m + n) % 2 == 1) return leftMax;

                int rightMin = 0;
                if (i == m) rightMin = nums2[j];
                else if (j == n) rightMin = nums1[i];
                else {
                    rightMin = min(nums1[i], nums2[j]);
                }
                return (leftMax + rightMin) * 0.5;
            }
        }
        return 0.0;
    }
};
// @lc code=end

