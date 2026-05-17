class Solution {
public:
    int mySqrt(int x) {
        for (long long i = 0; ; i++) {
            if (i * i > x) {
                return i - 1;
            }
        }
    }
};