#include <iostream>
#include <string>
#include <utility>
#include <vector>


typedef long long i64;
int main() {
    int n;
    std::cin >> n;
    std::vector<std::pair<i64, i64>> x_axis, y_axis;
    i64 cur_x = 0;
    i64 cur_y = 0;
    for (int i = 0; i < n; i++) {
        std::string query;
        std::cin >> query;
        if (query == "MOVE") {
            i64 a, b;
            std::cin >> a >> b;
            i64 next_x = cur_x + a, next_y = cur_y + b;
            x_axis.push_back(std::make_pair(std::min(cur_x, next_x), std::max(cur_x, next_x)));
            y_axis.push_back(std::make_pair(std::min(cur_y, next_y), std::max(cur_y, next_y)));
            cur_x = next_x;
            cur_y = next_y;
        } else if (query == "QUERY_EAST") {
            i64 p;
            std::cin >> p;
            int cnt = 0;
            for (auto &seg: x_axis) {
                i64 l = seg.first, r = seg.second;
                if (p > l && p < r) {
                    cnt++;
                }
            }
            std::cout << cnt << std::endl;
        } else {
            i64 q;
            std::cin >> q;
            int cnt = 0;
            for (auto &seg: y_axis) {
                i64 l = seg.first, r = seg.second;
                if (q > l && q < r) {
                    cnt++;
                }
            }
            std::cout << cnt << std::endl;
        }
    }
    return 0;
}