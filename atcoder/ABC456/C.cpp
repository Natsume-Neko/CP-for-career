#include <iostream>
#include <string>
int main() {
    using i64 = long long;
    std::string s;
    std::cin >> s;
    i64 cnt = 0;
    const int mod = 998244353;

    int left_pointer = 0;
    for (int i = 1; i < s.size(); i++) {
        if (s[i] == s[i - 1]) {
            cnt = (cnt + (i - left_pointer) * (i - left_pointer + 1) / 2) % mod;
            left_pointer = i;
        }
    }
    cnt = (cnt + (s.size() - left_pointer) * (s.size() - left_pointer + 1) / 2) % mod;
    std::cout << cnt << std::endl;
    return 0;
}