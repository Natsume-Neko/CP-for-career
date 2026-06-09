#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>


int main() {
    int n;
    std::cin >> n;
    std::string s;
    std::cin >> s;

    std::vector<std::vector<int>> prefix_sum(n + 1, std::vector<int>(3));
    std::vector<std::map<int, int>> diff(3);
    std::map<std::pair<int, int>, int> diff_all;
    std::vector<std::pair<int, int>> pairs{
        std::make_pair(0, 1),
        std::make_pair(0, 2),
        std::make_pair(1, 2)
    };

    long long result = 0;
    diff[0][0] = diff[1][0] = diff[2][0] = 1;
    diff_all[std::make_pair(0, 0)] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 3; j++) {
            prefix_sum[i + 1][j] = prefix_sum[i][j];
        }
        int ch = s[i] - 'A';
        prefix_sum[i + 1][ch]++;
        result += i + 1;

        for (int l = 0; l < 3; l++) {
            int j = pairs[l].first, k = pairs[l].second;
            int d = prefix_sum[i + 1][j] - prefix_sum[i + 1][k];
            result -= diff[l][d];
            diff[l][d]++;
        }
        int d1 = prefix_sum[i + 1][0], d2 = prefix_sum[i + 1][1], d3 = prefix_sum[i + 1][2];
        result += diff_all[std::make_pair(d2 - d1, d3 - d1)] * 2;
        diff_all[std::make_pair(d2 - d1, d3 - d1)]++;
        // std::cout << result << std::endl;
    }
    std::cout << result << std::endl;
    return 0;
}