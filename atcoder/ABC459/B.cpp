#include <iostream>
#include <string>
#include <vector>
int main() {
    int n;
    std::cin >> n;
    std::vector<int> ans(n);
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        auto c = s[0];
        if (c <= 'c') {
            ans[i] = 2;
        } else if (c <= 'f') {
            ans[i] = 3;
        } else if (c <= 'i') {
            ans[i] = 4;
        } else if (c <= 'l') {
            ans[i] = 5;
        } else if (c <= 'o') {
            ans[i] = 6;
        } else if (c <= 's') {
            ans[i] = 7;
        } else if (c <= 'v') {
            ans[i] = 8;
        } else {
            ans[i] = 9;
        }
    }
    for (int i = 0; i < n; i++) {
        std::cout << ans[i];
    }
    std::cout << std::endl;
    return 0;
}