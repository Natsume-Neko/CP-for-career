#include <iostream>
#include <utility>
#include <vector>
int main() {
    std::vector<std::pair<int, int>> directions{
        std::make_pair(-1, 0), 
        std::make_pair(1, 0), 
        std::make_pair(0, -1), 
        std::make_pair(0, 1)
    };
    int m, n;
    std::cin >> m >> n;
    std::vector<std::vector<bool>> visited(m, std::vector<bool>(n));
    int ans = 0;
    auto dfs = [&](this auto&& self, int x, int y, int cnt) {
        visited[x][y] = true;
        if (cnt == m * n) {
            ans++;
            visited[x][y] = false;
            return;
        }
        for (auto &direction: directions) {
            int next_x = x + direction.first;
            int next_y = y + direction.second;
            if (next_x >= 0 && next_x < m && next_y >= 0 && next_y < n && !visited[next_x][next_y]) {
                self(next_x, next_y, cnt + 1);
            }
        }
        visited[x][y] = false;
    };
    dfs(0, 0, 1);
    std::cout << ans << std::endl;
    return 0;
}