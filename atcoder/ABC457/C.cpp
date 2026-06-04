#include <iostream>
#include <vector>
int main() {
    int n;
    long long k;
    std::cin >> n >> k;
    std::vector<std::vector<int>> A(n);
    for (int i = 0; i < n; i++) {
        int l;
        std::cin >> l;
        A[i].resize(l);
        for (int j = 0; j < l; j++) {
            std::cin >> A[i][j];
        }
    }
    std::vector<int> C(n);
    for (int i = 0; i < n; i++) {
        std::cin >> C[i];
    }
    for (int i = 0; i < n; i++) {
        if (k <= 1ll * A[i].size() * C[i]) {
            int pos = (k - 1) % A[i].size();
            std::cout << A[i][pos] << std::endl;
            break;
        } else {
            k -= 1ll * A[i].size() * C[i];
        }
    }
    return 0;
}