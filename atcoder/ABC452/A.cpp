#include <iostream>
#include <set>
#include <utility>

int main() {
    const std::set<std::pair<int, int>> fes{
        std::make_pair(1, 7),
        std::make_pair(3, 3),
        std::make_pair(5, 5),
        std::make_pair(7, 7),
        std::make_pair(9, 9),
    };
    int M, D;
    std::cin >> M >> D;
    if (fes.find(std::make_pair(M, D)) != fes.end()) {
        std::cout << "Yes\n";
    } else {
        std::cout << "No\n";
    }
    return 0;
}