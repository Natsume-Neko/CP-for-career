#include <iostream>
#include <string>
#include <vector>
int main() {
    using i64 = long long;
    std::string s;
    std::cin >> s;
    std::vector<std::vector<i64>> f(s.size(), std::vector<i64>(27));
    const int mod = 998244353;

    f[0][s[0] - 'a'] = 1;
    f[0][26] = 1;
    for (int i = 1; i < s.size(); i++) {
        for (int j = 0; j < 27; j++) {
            f[i][j] = (f[i][j] + f[i - 1][j]) % mod;
            if (j != s[i] - 'a') {
                f[i][s[i] - 'a'] = (f[i][s[i] - 'a'] + f[i - 1][j]) % mod;
            }
        }
    }
    i64 result = 0;
    for (int i = 0; i < 26; i++) {
        result = (result + f[s.size() - 1][i]) % mod;
    }
    std::cout << result << std::endl;
    return 0;
}