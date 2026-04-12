# Coding Note for Binary Search

- [x]  LC 33 : 旋转数组
- [x]  LC 34 ：二分边界
- [x]  LC 875
- [x]  LC 162s

[toc]

## leetcode 33 Search in Rotated Sorted Array

好眼力！你一眼就看穿了 **$O(\log n)$** 这个复杂度的背后必有**二分查找（Binary Search）**。

### 🔍 你的想法评估：$O(n)$ 陷阱

作为教练，我必须客观地告诉你：**你的想法确实是 $O(n)$ 的。**

* **遍历找旋转点**：最坏情况下要看 $n$ 个数，复杂度 $O(n)$。
* **拼接数组**：且不说拼接需要 $O(n)$ 的空间和时间。
* **最后二分**：虽然二分是 $O(\log n)$，但前面的 $O(n)$ 已经拉低了整体的性能。

在算法竞赛中，如果题目要求 $O(\log n)$，通常意味着你**连一次完整的遍历都不被允许**。我们必须在“旋转”的状态下直接进行二分。

---

### 🧠 核心逻辑：旋转数组的“局部有序性”

旋转排序数组（如 `[4, 5, 6, 7, 0, 1, 2]`）有一个神奇的特性：**如果你从中间切一刀，左右两半中至少有一半是一定有序的。**

我们可以利用这一点：
1.  找到中点 `mid`。
2.  **判断哪一半是有序的**：
    * 如果 `nums[left] <= nums[mid]`，说明 **左半边** 是有序的。
    * 否则，**右半边** 是有序的。
3.  **判断 `target` 在不在有序的那一半里**：
    * 如果在，就把搜索范围收缩到那一半。
    * 如果不在，就去另一半（即便另一半是旋转的，逻辑依然成立）继续找。

[Image showing binary search on a rotated sorted array with one half always sorted]

---

### 💻 C++ 实现（直接二分）

```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = nums.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) return mid;

            // 情况 1：左半边 [left, mid] 是有序的
            if (nums[left] <= nums[mid]) {
                // 判断 target 是否在左半边的数值范围内
                if (nums[left] <= target && target < nums[mid]) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }
            // 情况 2：右半边 [mid, right] 是有序的
            else {
                // 判断 target 是否在右半边的数值范围内
                if (nums[mid] < target && target <= nums[right]) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
        return -1;
    }
};
```

---

### 🛡️ 教练纠点：为什么 `left + (right - left) / 2`？

你可能会写 `(left + right) / 2`。但在 ACM 中，如果 `left` 和 `right` 很大，相加可能会**整数溢出**。用减法再加法是职业选手的标准姿势。

### 💡 助记词
> **旋转二分不难搞，先看哪边是有序岛；**
> **目标若在岛内找，不在就往对岸跑。**

---

### 🚀 下一步挑战

搜索旋转数组还有个“变号”版（第 81 题），如果数组里有**重复数字**，这个二分逻辑就会失效（因为你无法通过 `nums[left] <= nums[mid]` 判断哪边有序）。

**你是想看看如何处理带重复数字的旋转二分，还是想试试另一个二分的经典应用：34. 在排序数组中查找元素的第一个和最后一个位置（寻找边界）？**

## leetcode 34 Find First and Last Position of Element in Sorted Array

Nice try! 你试图在一个 `while` 循环里通过 `if (result[0] != -1)` 这种打补丁的方式同时找左右边界，但这正是这道题最大的**逻辑陷阱**。

### 🔍 诊断报告：为什么你的代码没 AC？

1.  **逻辑冲突**：二分查找的核心是**折半**。当你找到一个 `target` 并记录在 `result[0]` 后，你让 `left = mid + 1` 去找第二个。但万一目标范围在 `mid` 的**左边**呢？或者 `target` 只有**一个**呢？你的 `break` 会导致你漏掉很多情况。
2.  **无法保证“最”边界**：二分查找找到的第一个 `target` 可能是这一堆重复数字中的**任何一个**（中间、偏左或偏右）。你的代码没有逻辑去保证 `result[0]` 一定是最左边的，`result[1]` 一定是最右边的。
3.  **找边界的本质**：在排序数组中找范围，本质上是做**两次**二分：一次找“第一个大于等于 target 的位置”，一次找“第一个大于 target 的位置”。

---

### 🧠 教练的“左闭右开”万能模版

既然你习惯 `[left, right)`，我们就用这个模版写两个函数，分别寻找**左侧边界**和**右侧边界**。

#### 1. 寻找左侧边界（Lower Bound）
关键点：找到 `target` 时，不要停！**继续向左收缩**（`right = mid`），直到压到最左边。

#### 2. 寻找右侧边界（Upper Bound）
关键点：找到 `target` 时，不要停！**继续向右收缩**（`left = mid + 1`），直到压到最右边。

---

### 💻 修正后的 C++ 代码

```cpp
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        if (nums.empty()) return {-1, -1};

        int first = findBound(nums, target, true);
        // 如果连左边界都找不到，右边界肯定也没有
        if (first == -1) return {-1, -1};
        
        int last = findBound(nums, target, false);
        return {first, last};
    }

private:
    // isFirst 为 true 找左边界，为 false 找右边界
    int findBound(vector<int>& nums, int target, bool isFirst) {
        int left = 0, right = nums.size(); // [left, right)
        int ans = -1;

        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] > target) {
                right = mid;
            } else if (nums[mid] < target) {
                left = mid + 1;
            } else {
                // 找到了 target，但不满足，继续挤压边界
                ans = mid;
                if (isFirst) {
                    right = mid;    // 找左边，所以向左挤压
                } else {
                    left = mid + 1; // 找右边，所以向右挤压
                }
            }
        }
        return ans;
    }
};
```

---

### 🛡️ 教练复盘：为什么这套模版无敌？

* **稳定性**：它不依赖于第一次碰巧撞到哪个 `target`。
* **语义清晰**：`right = mid` 就是在告诉程序“右边我不看了，我要看左边有没有更早出现的”。
* **符合习惯**：完全遵循你喜欢的 `[left, right)` 风格，退出循环时 `left == right`，逻辑自洽。

### 💡 助记词
> **二分找边界，相等别停下。**
> **找左缩右界，找右抬左跨。**

---

### 🚀 下一步计划

你现在已经掌握了：**简单二分**、**旋转二分**、以及最难的**边界二分**。

**二分查找的“武功”你已经练成 80% 了。剩下那 20% 是【二分答案法】（比如：875. 爱吃香蕉的珂珂）。你想领教一下这种“把二分用在结果集上”的高级操作吗？还是想休息一下，开启【递归与回溯】的脑暴之旅？**

## leetcode 875 Koko Eating Bananas

你的思维非常活跃，已经开始尝试从**局部特殊情况**推导一般规律了！但作为教练，我得赶紧把你从“排序后找值”的坑里拉出来，因为第 875 题的二分对象和你之前做的题**完全不同**。

---

### 🔍 你的思路缺陷诊断

1.  **关于 $H < n$ 的预判**：
    * 题目其实给了一个隐藏前提：$H \ge n$。因为如果 $H < n$，珂珂每小时吃一堆（这是最快的速度了）都吃不完。所以面试中你可以直接假设 $H \ge n$。
2.  **关于“排序后二分”的误区（重点）**：
    * **致命伤**：你之前的二分是在 `piles` 数组的**下标**或**元素值**上找。但这道题的答案（速度 $K$）**可能根本不在数组里**。
    * **例子**：`piles = [3, 6, 7, 11]`, $H = 8$。答案是 $K = 4$，但 4 并不在数组里。
    * **结论**：对 `piles` 排序对二分找 $K$ 几乎没有帮助，反而增加了 $O(n \log n)$ 的排序开销。

---

### 🧠 什么是“二分答案法”？

这道题的二分空间不是数组，而是**速度 $K$ 的取值范围**。

1.  **确定搜索范围**：
    * **最小值 $left$**：$1$（珂珂每小时最少吃 1 根）。
    * **最大值 $right$**：数组中的最大值（因为珂珂每小时吃掉一整堆，再快也没意义了）。
2.  **在这个 $[1, max(piles)]$ 的有序序列里二分**：
    * 我们不看数组下标，我们看的是“速度”。
    * 假设取中点 $mid$ 作为速度。
    * **判定条件（Check）**：以速度 $mid$ 吃完所有香蕉，总共需要多少小时？

---

### 💻 逻辑推演

假设 `piles = [3, 6, 7, 11]`, $H = 8$：
* $left = 1, right = 11 + 1$ (左闭右开)。
* 第一次 $mid = 6$：
    * 吃完用时：$1(3) + 1(6) + 2(7) + 2(11) = 6$ 小时。
    * $6 \le 8$，速度够快，我们要找“最小”，所以尝试**压低**速度：$right = 6$。
* 第二次 $mid = 3$：
    * 吃完用时：$1(3) + 2(6) + 3(7) + 4(11) = 10$ 小时。
    * $10 > 8$，速度太慢，必须**加速**：$left = 3 + 1 = 4$。

---

### 🛡️ 教练的 C++ 实现要点

```cpp
class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int left = 1;
        // 找到数组里的最大值作为右边界
        int right = 0;
        for (int p : piles) right = max(right, p);
        right++; // [left, right)

        while (left < right) {
            int mid = left + (right - left) / 2;
            if (canEatAll(piles, mid, h)) {
                right = mid; // 满足条件，尝试更小的速度
            } else {
                left = mid + 1; // 不满足，速度必须加快
            }
        }
        return left;
    }

private:
    bool canEatAll(const vector<int>& piles, int k, int h) {
        long hours = 0; // 防止相加溢出
        for (int p : piles) {
            // 计算当前堆需要的小时数：进一法除法
            hours += (p + k - 1) / k; 
        }
        return hours <= h;
    }
};
```

---

### 💡 总结：二分的三种境界

1.  **初级**：在有序数组里找一个存在的数。
2.  **中级**：在旋转或局部有序数组里找边界。
3.  **高级（二分答案）**：**题目没有给你有序数组，但你发现答案所在的范围是有序的。** 只要速度 $K$ 越大，耗时就越短。这种单调性就是二分的通行证。

**你对“二分答案”这种降维打击的思路感觉如何？如果这块骨头啃下来了，那我们就真的可以告别二分，去二叉树（Binary Tree）的递归森林里呼吸一下新鲜空气了！**

Nice try! 你的整体二分逻辑完全正确，但代码中有两个 C++ 语法层面的“致命伤”和一个性能上的“小坑”，导致了报错和可能的超时。

作为教练，我帮你一一揪出来：

### 🔍 诊断报告

#### 1. 语法错误：`max(piles)` 并不存在
在 C++ 中，你不能直接对 `vector` 使用 `max()`。
* **修正**：你需要使用 `<algorithm>` 库中的 `*max_element(piles.begin(), piles.end())`。

#### 2. 性能陷阱：`canEatAll` 里的 `while` 循环
* **问题**：你用 `while (pile > speed)` 不断减去速度来模拟吃香蕉。如果 `pile` 是 1 亿，而 `speed` 是 1，这个 `while` 会跑 1 亿次！这会导致 **TLE (超时)**。
* **修正**：直接用**除法**。吃完一堆的时间等于 `ceil(pile / speed)`，在整数运算中技巧性写法是：`(pile + speed - 1) / speed`。

#### 3. 潜在溢出：`times` 累加
* **问题**：虽然 $h$ 在 `int` 范围内，但如果速度很小，`times` 累加的结果可能会超过 `int` 的最大值（$2 \times 10^9$）。
* **修正**：将 `times` 声明为 `long` 或 `long long`。

---

### 💻 修正后的代码

```cpp
#include <algorithm>
#include <vector>

using namespace std;

class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int left = 1;
        // 修正 1：使用 *max_element 获取 vector 中的最大值
        int right = *max_element(piles.begin(), piles.end()) + 1;
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (canEatAll(piles, mid, h)) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        return left;
    }

private:
    bool canEatAll(const vector<int>& piles, int speed, int h) {
        long long times = 0; // 修正 3：防止累加溢出
        for (int pile : piles) {
            // 修正 2：用除法代替 while 循环，提高效率
            // (a + b - 1) / b 是向上取整的常用技巧
            times += (static_cast<long long>(pile) + speed - 1) / speed;
        }
        return times <= h;
    }
};
```

---

### 🛡️ 教练复盘：为什么要用 `(a + b - 1) / b`？

这是嵌入式和算法岗位面试中的一个小细节：
* 假设 `pile = 7`, `speed = 3`。
* 正常除法 `7 / 3 = 2`（余 1）。
* 珂珂需要 `2 + 1 = 3` 小时。
* 公式计算：`(7 + 3 - 1) / 3 = 9 / 3 = 3`。完美吻合！
* 这样避免了调用昂贵的浮点数函数 `ceil()`，在底层开发中更受欢迎。

### 💡 总结与展望

你已经成功跨过了**二分答案**这座大山！你的二分模版（左闭右开）用得非常熟练，这在处理复杂边界时会让你少走很多弯路。

**二分查找专题到此圆满结束！**

---

### 🌲 下一站：二叉树（Binary Tree）

二叉树是数据结构面试的“重灾区”，也是递归思想的最好载体。我们通常从这三类问题入手：
1.  **遍历问题**：前/中/后序遍历（递归与迭代）。
2.  **属性问题**：求深度、求节点数、判断平衡。
3.  **转化问题**：翻转二叉树（著名的 Homebrew 作者面试梗）、合并二叉树。

**你想从“递归三部曲”开始，还是先复习一下二叉树的各种遍历方式？**

## leetcode 162 Find Peak Element

非常精彩的问题！寻找峰值（162题）确实是二分查找中最令人惊叹的应用之一，因为它打破了“数组必须有序才能二分”的刻板印象。

作为教练，我先帮你解开这个“二分在哪”的谜团，再教你如何处理边界问题。

---

### 🧠 1. 核心逻辑：利用“局部单调性”

这道题的二分依据不是**数值**，而是**坡度（Slope）**。

想象你在爬山，你现在的坐标是 `mid`。
* 如果 `nums[mid] < nums[mid + 1]`：说明你在**上坡**。既然右边更高，那么右边一定存在一个峰值（最差的情况是数组一直增加到最后一个元素，那最后一个元素就是峰值，因为题目假设 `nums[n] = -∞`）。
* 如果 `nums[mid] > nums[mid + 1]`：说明你在**下坡**。那么你的左边（包括 `mid` 自己）一定存在一个峰值。

**这就是二分所在：每次比较，你都能排除掉一半的区间。**

---

### 🔍 2. 细节攻克：如何优雅地处理边界？

你担心的“对比左右两个元素会越界”是一个非常实际的问题。但在“左闭右开”或“比较 `mid` 与 `mid+1`”的策略下，有一个非常精妙的处理方法：

#### 秘诀：只比右邻居
在 `while (left < right)` 的框架下，由于 `mid` 是向下取整的，只要 `left < right` 且窗口大小至少为 2，`mid + 1` 就**一定不会**越界。我们只需要比较 `nums[mid]` 和 `nums[mid+1]`，就能知道往哪边走。

---

### 💻 修正后的 C++ 实现

```cpp
class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        int left = 0;
        int right = nums.size() - 1; // 这里用左闭右闭处理比较直观

        while (left < right) {
            int mid = left + (right - left) / 2;
            
            // 只要 mid 比它的右边邻居小，说明处于上坡
            // 峰值一定在右边（不包含 mid）
            if (nums[mid] < nums[mid + 1]) {
                left = mid + 1;
            } 
            // 否则，处于下坡（或 mid 本身就是峰值）
            // 峰值一定在左边（包含 mid）
            else {
                right = mid;
            }
        }
        
        // 最终 left == right，指向其中一个峰值
        return left;
    }
};
```

---

### 🛡️ 教练复盘：为什么这能行？

1.  **为什么不需要比左边？**
    * 因为我们是从左向右扫描坡度的。如果你发现 `mid` 比 `mid+1` 大（下坡），你收缩右边界到 `mid`。在下一次循环中，你依然在比较新的 `mid` 和它右边的邻居。这种“向上的趋势”会自动把你推向峰值。
2.  **收缩窗口的逻辑**：
    * `left = mid + 1`：排除掉矮的部分，往高处走。
    * `right = mid`：当前 `mid` 可能是峰值，所以不能是 `mid - 1`。

---

### 🎓 总结与进阶测验

你已经完成了二分查找的全部进阶逻辑。为了检验你对“边界”和“二分变体”的掌握，我为你准备了 10 道关于二分查找深度理解的测验题。


http://googleusercontent.com/immersive_entry_chip/0


完成这组练习，你对二分查找的理解就真的达到“工业级”水平了！

 Nice work! 你对算法细节的钻研精神非常棒。建议你完成这些练习后，继续探索更广阔的算法世界。你可以通过界面生成更多问题、一套知识卡片，或是定制一份学习指南！加油！