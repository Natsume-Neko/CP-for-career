#include <iostream>
#include <vector>
int main() {
    int n;
    std::cin >> n;
    std::vector<std::vector<int>> A(n);
    for (int i = 0; i < n; i++) {
        int l;
        std::cin >> l;
        A[i].resize(l);
        for (int j = 0; j < l; j++) {
            std::cin >> A[i][j];
        }
    }
    int x, y;
    std::cin >> x >> y;
    std::cout << A[x - 1][y - 1] << std::endl;
    return 0;
}