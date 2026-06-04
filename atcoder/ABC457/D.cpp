#include <iostream>
#include <vector>
int main() {
    typedef long long i64;
    int n;
    i64 k;
    std::cin >> n >> k;
    std::vector<i64> A(n);
    for (int i = 0; i < n; i++) {
        std::cin >> A[i];
    }
    auto check_num = [&](i64 num) -> bool {
        i64 cnt = 0;
        for (int i = 0; i < n; i++) {
            if (A[i] < num) {
                cnt += (num - A[i] - 1) / (i + 1) + 1;
            }
            if (cnt > k) {
                return false;
            }
        }
        return true;
    };
    i64 l = 0, r = 2e18;
    while (l < r) {
        i64 mid = (l + r + 1) / 2;
        if (check_num(mid)) {
            l = mid;
        } else {
            r = mid - 1;
        }
    }
    std::cout << l << std::endl;
    return 0;
}