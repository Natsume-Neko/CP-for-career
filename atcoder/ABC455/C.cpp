#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
int main() {
    using i64 = long long;
    int n, k;
    std::cin >> n >> k;
    std::vector<i64> A(n);
    std::map<int, i64> number_cnt;
    for (int i = 0; i < n; i++) {
        std::cin >> A[i];
        number_cnt[A[i]]++;
    }
    std::vector<i64> sums;
    for (auto number_pair: number_cnt) {
        sums.push_back(number_pair.first * number_pair.second);
    }
    std::sort(sums.begin(), sums.end(), std::greater<i64>());
    i64 result = 0;
    for (int i = k; i < sums.size(); i++) {
        result += sums[i];
    }
    std::cout << result << std::endl;
    return 0;
}