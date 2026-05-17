#include<vector>

class Solution {
public:
    int removeElement(std::vector<int>& nums, int val) {
        auto iter1 = nums.begin();
        auto iter2 = nums.begin();
        while (iter1 != nums.end()) {
            if (*iter1 != val) {
                *iter2 = *iter1;
                ++iter2;
            }
            ++iter1;
        }
        return iter2 - nums.begin();
    }
};