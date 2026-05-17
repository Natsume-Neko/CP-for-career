#include <vector>
class Solution {
public:
    void merge(std::vector<int>& nums1, int m, std::vector<int>& nums2, int n) {
        int l = 0, r = 0;
        std::vector<int> nums3;
        while (l < m || r < n) {
            if (l < m && r < n) {
                if (nums1[l] < nums2[r]) {
                    nums3.push_back(nums1[l]);
                    l++;
                } else {
                    nums3.push_back(nums2[r]);
                    r++;
                }
            } else if (l < m) {
                nums3.push_back(nums1[l]);
                l++;
            } else {
                nums3.push_back(nums2[r]);
                r++;
            }
        }
        for (int i = 0; i < n + m; i++) {
            nums1[i] = nums3[i];
        }
    }
};