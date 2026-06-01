#include <algorithm>
#include <iostream>
#include <vector>
int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<int> A(n), B(m);
    for (int i = 0; i < n; i++) {
        std::cin >> A[i];
    }
    for (int i = 0; i < m; i++) {
        std::cin >> B[i];
    }
    std::sort(A.begin(), A.end());
    std::sort(B.begin(), B.end());
    int i = n - 1, j = m - 1;
    int ans = 0;
    while (i >= 0 && j >= 0) {
        if (B[j] <= A[i] * 2) {
            ans++;
            j--;
            i--;
        } else {
            j--;
        }
    }
    std::cout << ans << std::endl;
}