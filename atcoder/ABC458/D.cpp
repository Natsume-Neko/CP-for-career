#include <functional>
#include <iostream>
#include <queue>
#include <vector>
int main() {
    int x, q;
    std::cin >> x >> q;
    std::priority_queue<int> less_than_x;
    std::priority_queue<int, std::vector<int>, std::greater<int>> greater_than_x;
    for (int i = 0; i < q; i++) {
        int a, b;
        std::cin >> a >> b;
        auto push = [&](int num) {
            if (num < x) {
                less_than_x.push(num);
            } else {
                greater_than_x.push(num);
            }
        };
        push(a);
        push(b);
        if (less_than_x.size() > greater_than_x.size()) {
            greater_than_x.push(x);
            x = less_than_x.top();
            less_than_x.pop();
        } else if (less_than_x.size() < greater_than_x.size()) {
            less_than_x.push(x);
            x = greater_than_x.top();
            greater_than_x.pop();
        }
        std::cout << x << std::endl;
    }
    return 0;
}