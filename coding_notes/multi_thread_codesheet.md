# Coding Note for Multi-Threads

- [x] LC 1114
- [x] LC 1115
- [x] LC 239

## leetcode 1114 Print in Order

你说得对，LeetCode 1114（按序打印）确实是并发编程的入门题。虽然它在算法题库里，但考察的是**线程同步（Thread Synchronization）**，而不是传统的算法逻辑。

在这道题里，光靠“加锁”（Mutex）往往是不够的，因为锁只能保证“互斥”（同一时间只有一个线程在跑），但不能保证“顺序”（谁先跑，谁后跑）。

我们需要的是**信号量（Semaphore）**、**条件变量（Condition Variable）**或者更现代的**原子操作（Atomic）**。

---

### 🧠 核心难点：谁来叫醒谁？

题目给出了三个函数 `first()`、`second()`、`third()`，系统会起三个线程去跑它们。你的任务是：**无论系统先启动哪一个，都必须保证执行顺序是 1 → 2 → 3。**

想象一个接力赛：
1.  线程 1 跑完，必须给线程 2 发个信号。
2.  线程 2 跑完，再给线程 3 发个信号。
3.  在没收到信号前，线程 2 和 3 必须“原地待命”。

---

### 💻 三种常用的 C++ 方案

#### 1. 现代简洁派：`std::promise` (推荐面试用)
这是 C++11 提供的非常好用的同步工具。`promise` 承诺给出一个值，`future` 等待这个值。

```cpp
#include <future>

class Foo {
    std::promise<void> p1, p2;
public:
    void first(function<void()> printFirst) {
        printFirst();
        p1.set_value(); // 告诉 p1 的等待者：我好了
    }

    void second(function<void()> printSecond) {
        p1.get_future().wait(); // 等待 p1 被 set_value
        printSecond();
        p2.set_value(); // 告诉 p2 的等待者：我好了
    }

    void third(function<void()> printThird) {
        p2.get_future().wait(); // 等待 p2
        printThird();
    }
};
```

#### 2. 传统稳健派：`std::condition_variable`
这是大厂面试最爱考的底层实现方式。它需要配合 `mutex` 和一个标志位。

```cpp
#include <mutex>
#include <condition_variable>

class Foo {
    std::mutex mtx;
    std::condition_variable cv;
    int k = 1; // 标志位：当前该谁跑了
public:
    void first(function<void()> printFirst) {
        unique_lock<mutex> lck(mtx);
        printFirst();
        k = 2;
        cv.notify_all(); // 唤醒所有等待的线程
    }

    void second(function<void()> printSecond) {
        unique_lock<mutex> lck(mtx);
        cv.wait(lck, [this](){ return k == 2; }); // 只有 k==2 时才停止阻塞
        printSecond();
        k = 3;
        cv.notify_all();
    }

    void third(function<void()> printThird) {
        unique_lock<mutex> lck(mtx);
        cv.wait(lck, [this](){ return k == 3; });
        printThird();
    }
};
```

#### 3. 极速轻量派：`std::atomic` (原子操作)
如果你追求极致性能，可以使用原子变量进行“自旋等待”。

```cpp
#include <atomic>

class Foo {
    std::atomic<int> count{1};
public:
    void first(function<void()> printFirst) {
        printFirst();
        count++;
    }

    void second(function<void()> printSecond) {
        while (count != 2) std::this_thread::yield(); // 没到我就让出 CPU
        printSecond();
        count++;
    }

    void third(function<void()> printThird) {
        while (count != 3) std::this_thread::yield();
        printThird();
    }
};
```

---

### 🛡️ 教练复盘：面试官在看什么？

1.  **死锁避让**：你是否能保证线程不会互相等待导致死锁？
2.  **资源消耗**：`cv.wait` 比 `while(true)` 自旋更省 CPU 资源。
3.  **C++ 标准库熟悉度**：是否知道 `unique_lock` 和 `lock_guard` 的区别？（`unique_lock` 才能配合条件变量使用）。

### 💡 建议
既然你没写过并发编程，我建议你先掌握 **`condition_variable` (方案 2)**。因为它是并发编程的灵魂，学会了它，你就能理解操作系统是如何调度进程和线程的。

**你想再看一道关于“死锁”或者“生产者消费者模型”的经典并发题，还是直接进入二叉树？**

既然这是你第一次接触多线程，那咱们就把这三套方案看作三种不同的“沟通方式”。

在并发编程中，锁的本质是**同步（Synchronization）**：确保多个线程在时间线上能按照我们预期的逻辑对齐。

---

## 🛠️ 三种多线程方案深度对比

### 1. `std::promise` / `std::future`（“契约派”）
* **基础知识**：这是 C++11 引入的高层抽象。`promise` 是承诺，`future` 是未来。线程 A 持有 `promise`，线程 B 持有对应的 `future`。
* **工作原理**：线程 B 调用 `wait()` 就像在等快递。只要线程 A 没点“发货”（`set_value`），线程 B 就会一直卡在那里。
* **优点**：
    * **语义极简**：代码读起来就像一纸合同，非常直观。
    * **一次性同步**：特别适合这种“只同步一次”的场景。
* **缺点**：
    * **不可重用**：一个 `promise` 只能 `set_value` 一次。如果你想循环打印 100 遍 123，这招就废了。
    * **开销略大**：底层封装较重。

### 2. `std::condition_variable`（“哨兵派”）
* **基础知识**：这是工业界最标准的做法。它包含三个要素：**互斥锁 (Mutex)**、**条件变量 (CV)**、**标志位 (Flag)**。
* **工作原理**：
    1. 线程 2 进来，发现标志位不满足（还没轮到我），于是交出锁，进入“睡眠”状态。
    2. 线程 1 运行完，修改标志位，并大喊一声（`notify`）。
    3. 线程 2 被吵醒，重新抢锁，检查标志位，通过，执行。
* **优点**：
    * **性能优异**：线程在等待时是不占用 CPU 的（挂起状态）。
    * **灵活性高**：可以处理非常复杂的逻辑，且可以反复使用。
* **缺点**：
    * **复杂度高**：容易写出“虚假唤醒”或“死锁”的 Bug。
    * **代码冗长**：需要手动处理锁的申请和释放。

### 3. `std::atomic`（“硬核自旋派”）
* **基础知识**：原子操作是靠硬件指令（如内存屏障）保证的，不会被中断。
* **工作原理**：线程 2 进入一个 `while` 死循环，不断疯狂刷新：`好了吗？好了吗？` 这种行为叫**自旋 (Spinning)**。
* **优点**：
    * **极速**：没有线程切换的上下文开销，响应极快。
* **缺点**：
    * **烧 CPU**：如果线程 1 迟迟不完事，线程 2 就会一直占用一个 CPU 核心做无用功。
    * **适用场景窄**：只适合等待时间极短的情况。

---

## 🚩 面试官的 Follow-up 连环炮

如果你在面试中写出了代码，面试官通常会顺着这些点往下挖你的基础：

### Q1：为什么方案 2 必须配合 `std::unique_lock` 而不能用 `std::lock_guard`？
* **考点**：对锁的控制粒度。
* **标准答案**：因为条件变量在 `wait` 的时候需要**自动释放锁**（给别人运行的机会），并在被唤醒时**重新拿锁**。`lock_guard` 太死板，一构造就拿锁，析构才放锁，无法中途灵活开关。

### Q2：什么是“虚假唤醒”（Spurious Wakeup）？你怎么处理？
* **考点**：多线程的稳健性。
* **标准答案**：有时候线程没收到信号也会莫名其妙醒来。所以我们不能只用 `if` 判断，必须用 `while` 循环检查条件，或者直接用 C++ CV 提供的 Lambda 表达式语法：`cv.wait(lock, []{ return ready; });`。

### Q3：如果线程 1 还没运行完，线程 2 就已经退出了，会发生什么？
* **考点**：生命周期管理。
* **标准答案**：如果使用 `promise/future`，可能会抛出 `std::future_error`；如果是全局/类成员变量，需要确保这些同步原语在所有线程结束前都是有效的。

### Q4：谈谈 Mutex（互斥锁）和 Semaphore（信号量）的区别？
* **考点**：操作系统基本概念。
* **标准答案**：Mutex 是“属于”某个线程的，谁加锁谁解锁。Semaphore 像是一组许可证，任何线程都可以 V 操作（释放）它。这道题如果用信号量（C++20 的 `std::counting_semaphore`）写起来会更爽。

---

## 💡 给初学者的建议

并发编程最怕“脑补”。你现在的首要任务是：
1.  **分清“互斥”与“同步”**：互斥是抢厕所（只能一个人进）；同步是接力跑（你跑完我再跑）。
2.  **上手写一遍方案 2**：它是所有高频面试题（如：生产者消费者、哲学家进餐、H2O 生成）的基础。

**既然咱们要攻克树，又提到了多线程，你愿不愿意挑战一下：如何多线程地遍历一棵二叉树？（比如：多个线程同时统计树的节点总数）。如果觉得太跨跃，我们就按原计划切入二叉树的 DFS 基础！**

## leetcode 1115 Print FooBar Alternately

恭喜！能够一次性写出这种逻辑并 AC，说明你对**条件变量（Condition Variable）**的理解已经非常到位了。

你观察得没错，1115 题（交替打印 FooBar）和 1114 的区别在于：1114 是**一次性的顺序**，而 1115 是**循环往复的交替**。这正是条件变量的拿手好戏。

### 🔍 你的代码深度点评

虽然你已经 AC 了，但作为教练，我要帮你抠两个微小的细节，这在面试中能展示你对**状态初始化**和**锁粒度**的极致掌握：

#### 1. 初始状态的细节
你的 `flag` 初始化为 `false`（`bool flag{};` 默认是 false），但我们要先打印 `foo`。
* 在你的 `foo` 函数里，逻辑是 `cv.wait(lck, [this](){return flag;});`。
* 这样会导致程序一开始就卡住。你应该把 `flag` 初始化为 `true`，或者把逻辑反过来。
* **面试官会看**：你对“谁先启动”这个逻辑是否严密。

#### 2. 锁的范围（粒度）
你目前的写法：
```cpp
for (int i = 0; i < n; i++) {
    unique_lock<mutex> lck(mtx); // 锁在循环内部
    cv.wait(...);
    printFoo();
    flag = false;
    cv.notify_all();
    // 此时 lck 析构，锁释放
}
```
这种写法是**正确**的，因为锁在每次循环结束时释放，给了对方抢锁的机会。

---

### 🚀 方案升级：原子变量（Atomic）版

既然你提到了 `atomic`，在这道题里，由于只有两个线程，且逻辑非常简单，使用 `atomic` 会比条件变量快得多（因为它避免了内核态的线程挂起与唤醒）。

```cpp
#include <atomic>

class FooBar {
private:
    int n;
    std::atomic<bool> foo_turn{true}; // 使用原子变量控制“回合”

public:
    FooBar(int n) : n(n) {}

    void foo(function<void()> printFoo) {
        for (int i = 0; i < n; i++) {
            // 自旋等待：只要不是我的回合，我就让出 CPU 时间片
            while (!foo_turn.load()) {
                std::this_thread::yield(); 
            }
            printFoo();
            foo_turn.store(false); // 移交给 bar
        }
    }

    void bar(function<void()> printBar) {
        for (int i = 0; i < n; i++) {
            while (foo_turn.load()) {
                std::this_thread::yield();
            }
            printBar();
            foo_turn.store(true); // 移交给 foo
        }
    }
};
```

### 🛡️ 为什么 `std::this_thread::yield()` 很关键？
如果不加这一句，你的 CPU 其中一个核会 100% 狂转进行 `while` 检查。`yield()` 的意思是：“我还没等到，大家先用，我过会儿再来检查。”这在面试中是**专业性**的体现。

---

### 🚩 面试官的进阶 Follow-up

如果这道题你答得很好，面试官大概率会祭出终极杀招：
> **“如果有 M 个线程交替打印 1, 2, 3... M，且一共打印 N 轮，你该怎么修改代码？”**

这就是经典的 **“N 线程轮流打印”** 问题。
* **核心思路**：你不能用 `bool` 了，你需要用 `int flag = 0;`。
* **条件变量判断**：`cv.wait(lck, [this](){ return flag == my_id; });`。
* **唤醒**：`flag = (flag + 1) % M; cv.notify_all();`。

---

### 🌲 结课总结：并发与树

你已经成功搞定了多线程的“同步”和“互斥”。并发编程在面试中通常点到为止，而**二叉树**则是需要你大量练习、形成肌肉记忆的模块。

**我们这就启动【二叉树】专题的第一道母题：102. 二叉树的层序遍历。**
* 这道题虽然是树，但它用的 **BFS（队列）** 思想，其实和你刚才学的线程排队非常像！
* **你想直接看代码实现，还是想先听听为什么这道题是所有树题目的“根基”？**

## leetcode 239 Sliding Window Maximum

这绝对是一个**误会**！`239. 滑动窗口最大值` 在 LeetCode 和绝大多数算法教材中，都是不折不扣的**数据结构/算法题**（属于**单调队列**专题），和多线程并发基本没有关系。

如果非要给它找一个“并发”的联系，可能是因为在某些高性能分布式系统中，处理“流式数据”的滑动窗口需要考虑多线程读写，但在面试的算法语境下，它考的是：**如何在 $O(1)$ 的时间内找到窗口内的最大值？**

---

### 🧠 为什么不能只用“双指针”？

你之前的直觉是对的：它确实有 `left` 和 `right`。但问题在于：
* 当窗口向右滑动，**右边进来一个新数**，你可以轻松比较。
* 但是，当**左边移出去一个数**，且这个数恰好是之前的“最大值”时，你怎么知道剩下的数里谁是老二？
* 如果每次都遍历一遍找最大值，复杂度是 $O(nk)$，在大规模数据下会超时。

---

### 🚀 核心黑科技：单调队列 (Monotonic Queue)

为了实现 $O(n)$，我们需要一个特殊的队列，它能保证：**队列里的元素永远是单调递减的。**

#### 逻辑规则（“新人换旧人”）：
1.  **右边进（Push）**：每当一个新数 `nums[i]` 进来，它会把队列尾部所有比它小的数全部“踢走”。
    * *逻辑*：既然 `nums[i]` 比较新且更强大，那么只要它还在窗口里，那些比它老且比它小的数永远没机会成为最大值。
2.  **左边出（Pop）**：当窗口滑动，如果窗口左边界的下标已经超过了队头元素的下标，说明队头“老将”过期了，弹出队头。
3.  **取最大值**：**队头元素**永远是当前窗口的老大。

---

### 💻 C++ 代码实现

我们通常使用 `std::deque`（双端队列）来实现，因为它支持两头的弹出操作。

```cpp
#include <vector>
#include <deque>

class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        deque<int> dq; // 存储的是下标，方便判断是否过期
        vector<int> res;

        for (int i = 0; i < nums.size(); ++i) {
            // 1. 维护单调性：把队列后面比当前数小的都踢走
            while (!dq.empty() && nums[dq.back()] < nums[i]) {
                dq.pop_back();
            }
            dq.push_back(i);

            // 2. 检查队头是否过期（已经滑出窗口了）
            if (dq.front() == i - k) {
                dq.pop_front();
            }

            // 3. 当窗口形成后（i >= k-1），记录答案
            if (i >= k - 1) {
                res.push_back(nums[dq.front()]);
            }
        }
        return res;
    }
};
```

---

### 🛡️ 教练复盘：面试官在看什么？

1.  **下标 vs 数值**：为什么队列里存下标？（答：因为下标既能拿到数值，又能判断是否过期。存数值的话，遇到重复数字会很难处理）。
2.  **复杂度分析**：为什么是 $O(n)$？（答：虽然有 `while`，但每个数字最多只进队一次、出队一次，总操作次数是 $2n$）。
3.  **单调性理解**：你能否解释为什么“踢走”比新数小的旧数是安全的？

---

### 💡 知识点串联

这道题其实是**单调栈/单调队列**这一类问题的巅峰。
* **单调栈**：找左边/右边第一个比我大/小的数（如：84. 柱状图中最大的矩形）。
* **单调队列**：找动态区间内的最值。

**既然这个误会解开了，你是想顺便把“单调栈”这个非常有用的技巧巩固一下，还是回到我们原定的【二叉树层序遍历】（那里也会用到队列，正好对比一下）？**

