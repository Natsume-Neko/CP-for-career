#include <iostream>
#include <string>
int main() {
    std::string s;
    int n;
    std::cin >> s >> n;
    for (int i = n; i < s.size() - n; i++) {
        std::cout << s[i];
    }
    std::cout << std::endl;
    return 0;
}