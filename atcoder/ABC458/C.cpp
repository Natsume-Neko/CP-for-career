#include <algorithm>
#include <iostream>
#include <string>
int main() {
    std::string s;
    std::cin >> s;
    long long num_of_subsequence = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == 'C') {
            num_of_subsequence += std::min(i + 1, (int)s.size() - i);
        }
    }
    std::cout << num_of_subsequence << std::endl;
}