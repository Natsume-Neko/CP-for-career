#include <iostream>
#include <string>

int main() {
    std::string s;
    std::cin >> s;
    auto i = s.cbegin();
    auto j = s.cbegin();
    auto k = s.cbegin();
    int res = 0;
    while (i != s.cend() && j != s.cend() && k != s.cend()) {
        while (i != s.cend() && *i != 'A') {
            ++i;
        }
        while (j != s.cend() && (j <= i || *j != 'B')) {
            ++j;
        }
        while (k != s.cend() && (k <= j || *k != 'C')) {
            ++k;
        }
        if (i != s.cend() && j != s.cend() && k != s.cend()) {
            ++res;
            ++i;
            ++j;
            ++k;
        }
    }
    std::cout << res << '\n';
    return 0;
}