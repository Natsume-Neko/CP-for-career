#include <vector>

class Solution {
public:
    int trap(std::vector<int>& height) {
        int ans = 0;
        auto iter = height.begin();
        auto max_iter = height.begin();
        while (iter != height.end()) {
            if (*iter > *max_iter) {
                max_iter = iter;
            }
            ++iter;
        }
        iter = height.begin();
        int cur_max = 0;
        while (iter < max_iter) {
            if (*iter > cur_max) {
                cur_max = *iter;
            } else {
                ans += cur_max - *iter;
            }
            ++iter;
        }
        iter = height.end();
        --iter;
        cur_max = 0;
        while (iter > max_iter) {
            if (*iter > cur_max) {
                cur_max = *iter;
            } else {
                ans += cur_max - *iter;
            }
            --iter;
        }
        return ans;
    }
};