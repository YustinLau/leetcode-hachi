// 合并两个无序数组
// 最终输出为一个有序数组

#include <iostream>
#include <algorithm>
using namespace std;

// 方法一：堆排序返回结果
vector<int> SortVectorHeap(vector<int> nums1, vector<int> nums2){
    priority_queue<int, vector<int>, greater<int>> pq;
    for (auto num1 : nums1) {
        pq.push(num1);
    }
    for (auto num2 : nums2) {
        pq.push(num2);
    }
    vector<int> result;
    while (!pq.empty()) {
        int element = pq.top();
        result.push_back(element);
        pq.pop();
    }
    return result;
}

// 方法2: 分别排序 双指针构造
vector<int> SortVectorOrdered(vector<int> nums1, vector<int> nums2){
    sort(nums1.begin(), nums1.end());
    sort(nums2.begin(), nums2.end());
    // 依次比较最小值再合并
    // 合并可以原地双指针修改nums1减少空间开销 fast slow
    // 构造两个有序再合并与直接合并再sort开销类似

}

// 
vector<int> SortVectorQuickSort(vector<int> nums1, vector<int> nums2){
    // 直接合并在一起然后快速排序
    // 快排privot
    for (auto num : nums2) {
        nums1.push_back(num);
    }
    
}

vector<int> SortVectorSingleStack(vector<int> nums1, vector<int> nums2){
    // 借用单调栈维持顺序，但需额外开辟空间 ～= Heap
}
