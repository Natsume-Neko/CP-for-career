#include <iostream>
#include <vector>
int main() {
    int n, q;
    std::cin >> n >> q;
    std::vector<int> cells(n + 1), greater(q + 1);
    int removed = 0;
    for (int i = 0; i < q; i++) {
        int type, x;
        std::cin >> type >> x;
        if (type == 1) {
            cells[x]++;
            greater[cells[x]]++;
            if (cells[x] == removed + 1 && greater[removed + 1] == n) {
                removed++;
            }
        } else {
            if (x + removed > q) {
                std::cout << 0 << std::endl;
            } else {
                std::cout << greater[x + removed] << std::endl;
            }
        }
    }
}