/*
 * @lc app=leetcode.cn id=1115 lang=cpp
 *
 * [1115] 交替打印 FooBar
 */

// @lc code=start
#include <atomic>
using namespace std;
class FooBar {
private:
    int n;
    atomic<bool> foo_turn{true};

public:
    FooBar(int n) {
        this->n = n;
    }

    void foo(function<void()> printFoo) {
        
        for (int i = 0; i < n; i++) {
            while (!foo_turn.load()) {
                this_thread::yield();
            }
        	// printFoo() outputs "foo". Do not change or remove this line.
        	printFoo();
            foo_turn.store(false);
        }
    }

    void bar(function<void()> printBar) {
        
        for (int i = 0; i < n; i++) {
            while (foo_turn.load()) {
                this_thread::yield();
            }
        	// printBar() outputs "bar". Do not change or remove this line.
        	printBar();
            foo_turn.store(true);
        }
    }
};
// @lc code=end

