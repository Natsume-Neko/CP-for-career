#include <iostream>
#include <string>

int main() {
    std::string s, t;
    std::cin >> s >> t;
    int res = 0;
    auto iter_s = s.cbegin();
    auto iter_t = t.cbegin();
    while (iter_s != s.cend() || iter_t != t.cend()) {
        if (iter_s != s.cend() && iter_t != t.cend() && *iter_s == *iter_t) {
            ++iter_s;
            ++iter_t;
        } else if (iter_s != s.cend() && *iter_s == 'A') {
            ++iter_s;
            ++res;
        } else if (iter_t != t.cend() && *iter_t == 'A') {
            ++iter_t;
            ++res;
        } else {
            std::cout << "-1\n";
            return 0;
        }
    }
    std::cout << res << '\n';
    return 0;
}