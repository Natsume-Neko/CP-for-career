#include <cstdint>
#include <functional>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

const int64_t mod = 998244353;

int main() {
    int N, M;
    std::cin >> N >> M;
    auto mod_2_list = std::vector<int64_t>(M + 1);
    mod_2_list[0] = 1;
    int64_t res = 0;
    for (int i = 1; i <= M; i++) {
        mod_2_list[i] = mod_2_list[i - 1] * 2 % mod;
    }
    auto edges = std::vector<std::pair<int, int>>(M);
    for (int i = 0; i < M; i++) {
        std::cin >> edges[i].first >> edges[i].second;
    }
    
    auto dsu = std::vector<int>(N + 1);
    std::iota(dsu.begin(), dsu.end(), 0);
    std::function<int(int)> find = [&](int x) {
        if (dsu[x] == x) return x;
        return dsu[x] = find(dsu[x]);
    };
    std::function<void(int, int)> merge = [&](int x, int y) {
        dsu[find(x)] = dsu[find(y)];
    };

    int cnt = N;
    for (int i = M - 1; i >= 0; i--) {
        int u = edges[i].first;
        int v = edges[i].second;
        if (cnt > 2 || find(u) == find(v)) {
            if (find(u) != find(v)) {
                cnt--;
                merge(u, v);
            } 
        } else {
            res = (res + mod_2_list[i + 1]) % mod;
        }
    }
    std::cout << res << '\n';
    return 0;
}