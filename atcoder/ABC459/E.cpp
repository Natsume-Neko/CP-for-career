#include <iostream>
#include <vector>
typedef long long i64;
// Binary exponentiation.
i64 pow(i64 a, i64 b, i64 m) {
  long long res = 1, po = a;
  for (; b; b >>= 1) {
    if (b & 1) res = res * po % m;
    po = po * po % m;
  }
  return res;
}

// Returns the modular inverse of a prime modulo p.
i64 inverse(i64 a, i64 p) { return pow(a, p - 2, p); }

int main() {
    int n;
    std::cin >> n;
    std::vector<std::vector<int>> edges(n + 1);
    for (int i = 2; i <= n; i++) {
        int parent;
        std::cin >> parent;
        edges[parent].push_back(i);
    }
    std::vector<i64> c(n + 1);
    for (int i = 1; i <= n; i++) {
        std::cin >> c[i];
    }
    std::vector<i64> d(n + 1);
    for (int i = 1; i <= n; i++) {
        std::cin >> d[i];
    }
    i64 ans = 1;
    const int mod = 998244353;
    auto dfs = [&](this auto&& self, int cur) -> void {
        for (auto next: edges[cur]) {
            self(next);
            c[cur] += c[next];
        }
        if (c[cur] < d[cur]) {
            c[cur] = 0;
            ans = 0;
        } else {
            for (i64 num = c[cur]; num > c[cur] - d[cur]; num--) {
                ans = ans * (num % mod) % mod;
            }
            for (i64 num = d[cur]; num > 1; num--) {
                ans = ans * inverse(num, mod) % mod;
            }
            c[cur] -= d[cur];
        }
    };
    dfs(1);
    std::cout << ans << std::endl;
}