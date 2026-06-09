#include <iostream>
int main() {
    int x;
    std::cin >> x;
    if (x >= 3 && x <= 18) {
        std::cout << "Yes\n";
    } else {
        std::cout << "No\n";
    }
}