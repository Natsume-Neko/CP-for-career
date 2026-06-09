#include <iostream>
#include <string>
#include <vector>
int main() {
    int t;
    std::cin >> t;
    while (t--) {
        int n, m;
        std::cin >> n >> m;
        std::vector<std::vector<int>> edges(n + 1);
        for (int i = 0; i < m; i++) {
            int u, v;
            std::cin >> u >> v;
            edges[u].push_back(v);
            edges[v].push_back(u);
        }
        int w;
        std::cin >> w;
        std::vector<std::vector<bool>> holidays(n + 1, std::vector<bool>(w + 1));
        for (int i = 1; i <= n; i++) {
            std::string s;
            std::cin >> s;
            for (int j = 0; j < w; j++) {
                holidays[i][j + 1] = (s[j] == 'o');
            }
        }
        std::vector<std::vector<bool>> visited(n + 1, std::vector<bool>(w + 1)), is_visiting(n + 1, std::vector<bool>(w + 1));
        auto dfs = [&](this auto &&self, int node, int day) -> bool {
            if (is_visiting[node][day]) {
                return true;
            }
            is_visiting[node][day] = true;
            visited[node][day] = true;
            int next_day = day % w + 1;
            for (auto next: edges[node]) {
                if (holidays[next][next_day]) {
                    if (self(next, next_day)) return true;
                }
            }
            if (holidays[node][next_day]) {
                if (self(node, next_day)) return true;
            }
            is_visiting[node][day] = false;
            return false;
        };
        bool is_existing = false;
        for (int i = 1; i <= n; i++) {
            if (!visited[i][1] && holidays[i][1]) {
                if (dfs(i, 1)) {
                    is_existing = true;
                    break;
                }
            }
        }
        if (is_existing) {
            std::cout << "Yes\n";
        } else {
            std::cout << "No\n";
        }
    }

    return 0;
}