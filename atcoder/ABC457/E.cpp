#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::multiset<int>> left_container(n + 1), right_container(n + 1);
    for (int i = 0; i < m; i++) {
        int l, r;
        std::cin >> l >> r;
        left_container[l].insert(r);
        right_container[r].insert(l);
    }

    std::vector<int> logn(2);
    logn.push_back(1);
    for (int i = 3; i <= n; i++) {
        logn.push_back(logn[i / 2] + 1);
    }
    int logN = logn[n];
    std::vector<std::vector<int>> min_left(logN + 1, std::vector<int>(n + 1));
    for (int i = 1; i <= n; i++) {
        if (left_container[i].size()) {
            min_left[0][i] = *left_container[i].begin();
        } else {
            min_left[0][i] = n + 1;
        }
    }
    for (int i = 1; i <= logN; i++) {
        for (int j = 1; j + (1 << i) - 1 <= n; j++) {
            min_left[i][j] = std::min(min_left[i - 1][j], min_left[i - 1][j + (1 << (i - 1))]);
        }
    }
    auto search_minimum = [&](int l, int r) -> int {
        int s = logn[r - l + 1];
        return std::min(min_left[s][l], min_left[s][r - (1 << s) + 1]);
    };

    int q;
    std::cin >> q;
    while (q--) {
        int s, t;
        std::cin >> s >> t;
        auto iter_left = left_container[s].upper_bound(t);
        if (left_container[s].begin() == iter_left) {
            std::cout << "No" << std::endl;
            continue;
        }
        --iter_left;
        auto iter_right = right_container[t].lower_bound(s);
        if (right_container[t].end() == iter_right) {
            std::cout << "No" << std::endl;
            continue;
        }
        if (*iter_left >= *iter_right - 1) {
            if (*iter_left == t && *iter_right == s) {
                bool flag = false;
                auto iter_left_minimum = left_container[s].begin();
                if (*iter_left_minimum != t) {
                    flag = true;
                } else {
                    ++iter_left_minimum;
                    if (iter_left_minimum != left_container[s].end() && *iter_left_minimum == t) {
                        flag = true;
                    }
                }
                auto iter_right_maximum = right_container[t].end();
                --iter_right_maximum;
                if (*iter_right_maximum != s) {
                    flag = true;
                }
                if (s < t - 1 && search_minimum(s + 1, t - 1) < t) {
                    flag = true;
                }
                if (flag) {
                    std::cout << "Yes" << std::endl;
                } else {
                    std::cout << "No" << std::endl;
                }
            } else {
                std::cout << "Yes" << std::endl;
            }
        } else {
            std::cout << "No" << std::endl;
        }
    }
    return 0;
}