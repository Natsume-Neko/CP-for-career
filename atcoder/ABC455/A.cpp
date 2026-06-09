#include <iostream>
int main() {
    int A, B, C;
    std::cin >> A >> B >> C;
    if (A != B && B == C) {
        std::cout << "Yes\n";
    } else {
        std::cout << "No\n";
    }
    return 0;
}