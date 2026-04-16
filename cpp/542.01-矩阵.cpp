/*
 * @lc app=leetcode.cn id=542 lang=cpp
 *
 * [542] 01 矩阵
 */

// @lc code=start
#include <vector>
#include <queue>
using namespace std;
class Solution {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        int m = mat.size();
        int n = mat[0].size();
        // 1. 初始化结果矩阵，用一个特殊值（如 -1）表示未访问
        vector<vector<int>> dist(m, vector<int>(n, -1));
        queue<pair<int, int>> q;

        // 2. 将所有 0 放入队列，作为 BFS 的第一层
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (mat[i][j] == 0) {
                    dist[i][j] = 0;
                    q.push({i, j});
                }
            }
        }

        // 四个方向的偏移量：上、下、左、右
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        // 3. 开始多源 BFS 扩散
        while (!q.empty()) {
            auto [curr_x, curr_y] = q.front();
            q.pop();

            for (int i = 0; i < 4; ++i) {
                int next_x = curr_x + dx[i];
                int next_y = curr_y + dy[i];

                // 检查边界 且 必须是未访问过的点（dist == -1）
                if (next_x >= 0 && next_x < m && next_y >= 0 && next_y < n 
                    && dist[next_x][next_y] == -1) {
                    
                    // 距离 = 父亲距离 + 1
                    dist[next_x][next_y] = dist[curr_x][curr_y] + 1;
                    q.push({next_x, next_y});
                }
            }
        }

        return dist;
    }
};
// @lc code=end

