#include <algorithm>
#include <iostream>
int main() {
    int t;
    std::cin >> t;
    while (t--) {
        typedef long long i64;
        i64 x1, y1, r1, x2, y2, r2;
        std::cin >> x1 >> y1 >> r1 >> x2 >> y2 >> r2;
        i64 distance_square = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
        if (distance_square <= (r1 + r2) * (r1 + r2) && distance_square >= (std::min(r1, r2) - std::max(r1, r2)) * (std::min(r1, r2) - std::max(r1, r2))) {
            std::cout << "Yes\n";
        } else {
            std::cout << "No\n";
        }
    }
    return 0;
}