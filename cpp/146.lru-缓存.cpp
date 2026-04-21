/*
 * @lc app=leetcode.cn id=146 lang=cpp
 *
 * [146] LRU 缓存
 */

// @lc code=start
#include <unordered_map>
#include <list>
using namespace std;
class LRUCache {
private:
    int capacity{0};
    list<pair<int, int>> data;
    unordered_map<int, list<pair<int, int>>::iterator> records;
public:
    LRUCache(int n) : capacity(n) {
        
    }
    
    int get(int key) {
        if (records.find(key) == records.end()) return -1;
        data.splice(data.begin(), data, records[key]);
        return records[key]->second;
    }
    
    void put(int key, int value) {
        if (records.find(key) != records.end()) {
            data.splice(data.begin(), data, records[key]);
            records[key]->second = value;
        } else {
            if (records.size() == capacity) {
                int element = data.back().first;
                data.pop_back();
                records.erase(element);
            }
            data.push_front({key, value});
            records[key] = data.begin();
        }
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */
// @lc code=end

