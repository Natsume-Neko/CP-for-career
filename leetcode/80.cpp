#include <vector>

class Solution {
public:
    int removeDuplicates(std::vector<int>& nums) {
        int cnt = 0;
        int lst = -1000000;
        int l = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] != lst) {
                cnt = 1;
                lst = nums[i];
                nums[l] = nums[i];
                l++;
            } else {
                cnt++;
                if (cnt <= 2) {
                    nums[l] = nums[i];
                    l++;
                }
            }
        }
        return l;
    }
};