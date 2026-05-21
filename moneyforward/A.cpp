#include <algorithm>
#include <iostream>
#include <vector>
int main() {
    int n, k;
    std::cin >> n >> k;
    std::vector<int> A(n), B(n), days(n);
    for (int i = 0; i < n; i++) {
        std::cin >> A[i];
    }
    for (int i = 0; i < n; i++) {
        std::cin >> B[i];
    }
    for (int i = 0; i < n; i++) {
        if (A[i] < k) {
            days[i] = (k - A[i] - 1) / B[i] + 1;
        }
    }
    std::sort(days.begin(), days.end());
    std::cout << days[(n + 1) / 2 - 1] << std::endl;
    return 0;
}