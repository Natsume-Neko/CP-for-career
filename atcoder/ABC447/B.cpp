#include <algorithm>
#include <iostream>
#include <string>
#include <array>

int main() {
    std::string s;
    std::cin >> s;
    auto count = std::array<int, 26>();
    int mx = 0;
    for (auto c: s) {
        count[c - 'a'] += 1;
        mx = std::max(mx, count[c - 'a']);
    }
    std::erase_if(s, [&](char c) {
        return count[c - 'a'] == mx;
    });
    std::cout << s << '\n';
}