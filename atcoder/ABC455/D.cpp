#include <iostream>
#include <vector>
int main() {
    int n, q;
    std::cin >> n >> q;
    std::vector<int> parent(n + 1);
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
    }
    for (int i = 0; i < q; i++) {
        int c, p;
        std::cin >> c >> p;
        parent[c] = p;
    }
    std::vector<int> size(n + 1);
    std::vector<bool> have_in_degree(n + 1, false);
    for (int i = 1; i <= n; i++) {
        if (i != parent[i]) {
            have_in_degree[parent[i]] = true;
        }
    }
    for (int i = 1; i <= n; i++) {
        if (!have_in_degree[i]) {
            int cnt = 1, cur = i;
            while (parent[cur] != cur) {
                cnt++;
                cur = parent[cur];
            }
            size[cur] = cnt;
        }
    }
    for (int i = 1; i <= n; i++) {
        std::cout << size[i] << ' ';
    }
    std::cout << "\n";
    return 0;
}