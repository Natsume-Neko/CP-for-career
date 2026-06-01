#include <iostream>
int main() {
    int n, m;
    std::cin >> n >> m;
    int ans = 0;
    while (m) {
        m = n % m;
        ans++;
    }
    std::cout << ans << std::endl;
    return 0;
}