#include <iostream>
int main() {
    std::string s = "HelloWorld";
    int x;
    std::cin >> x;
    s.erase(s.begin() + x - 1);
    std::cout << s << std::endl;
    return 0;
}