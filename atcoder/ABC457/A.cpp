#include <iostream>
#include <vector>
int main() {
    int n;
    std::cin >> n;
    std::vector<int> A(n);
    for (int i = 0; i < n; i++) {
        std::cin >> A[i];
    }
    int x;
    std::cin >> x;
    std::cout << A[x - 1] << std::endl;
    return 0;
}