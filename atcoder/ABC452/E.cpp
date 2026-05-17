#include <iostream>
#include <numeric>
#include <vector>

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<long long> A(n), B(m);
    for (int i = 0; i < n; i++) {
        std::cin >> A[i];
        A[i] *= i + 1;
    }
    for (int i = 0; i < m; i++) {
        std::cin >> B[i];
        B[i] %= i + 1;
    }
    const long long mod = 998244353;
    long long ans = (std::reduce(A.begin(), A.end()) + std::reduce(B.begin(), B.end())) % mod;
    std::cout << ans << std::endl;
    return 0;
}