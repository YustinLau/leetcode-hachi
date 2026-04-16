/*
 * @lc app=leetcode.cn id=210 lang=cpp
 *
 * [210] 课程表 II
 */

// @lc code=start
#include <vector>
#include <queue>
using namespace std;
class Solution {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> adj(numCourses);
        vector<int> indegree(numCourses, 0);
        vector<int> result;
        for (auto pair : prerequisites) {
            adj[pair[1]].push_back(pair[0]);
            indegree[pair[0]]++;
        }
        queue<int> q;
        for (int i = 0; i < numCourses; i++) {
            if (indegree[i] == 0) {
                q.push(i);
            }
        }
        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            result.push_back(curr);
            for (int next : adj[curr]) {
                indegree[next]--;
                if (indegree[next] == 0) {
                    q.push(next);
                }
            }
        }
        return result.size() == numCourses ? result : vector<int>();

    }
};
// @lc code=end

