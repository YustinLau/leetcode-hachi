/*
 * @lc app=leetcode.cn id=2079 lang=cpp
 *
 * [2079] 给植物浇水
 */

// @lc code=start
#include <vector>
using namespace std;
class Solution {
public:
    int wateringPlants(vector<int>& plants, int capacity) {
        int steps = 0;
        int res = capacity;
        for (int i = 0; i < plants.size(); i++) {
            if (plants[i] <= res) {
                steps += 1;
                res -= plants[i];
            } else {
                steps += 2 * i + 1;
                res = capacity - plants[i];
            }
        }
        return steps;
    }
};
// @lc code=end

