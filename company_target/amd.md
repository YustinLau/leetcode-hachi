# Coding Note for AMD 

[toc]

## CHECKLIST :

- [x] leetcode 215 数组中的第K个最大元素
- [x] leetcode 746 使用最小花费爬楼梯
- [x] leetcode 2079 给植物浇水
- [x] leetcode LCR 188 买卖芯片的最佳时机
- [x] leetcode 02.03 删除中间节点

## leetcode 215

- priority_queue
- `greater<int>`

## leetcode 746

- dp
- dp[0] = dp[1] = 0
- i = 2开始
- min(cost i-1, cost i-2)

## leetcode 2079

- 审题注意：`max(plants[i]) <= capacity`
- 折腾回去的步数计算：steps += 2 * i + 1; 
- 按模拟分情况实现即可

## leetcode LCR 188

- 维持最低价和最大利润变量
- 每次遍历刷新即可，重复题目较多注意区分


## leetcode 02.03

- 找前驱比较难，反过来让当前等于next



