#include <iostream>
#include <string>
#include <vector>
int main() {
    int h, w;
    std::cin >> h >> w;
    std::vector<std::string> grids(h);
    for (int i = 0; i < h; i++) {
        std::cin >> grids[i];
    }

    auto check = [&](int h1, int w1, int h2, int w2) -> bool {
        for (int i = h1; i <= h2; i++) {
            for (int j = w1; j <= w2; j++) {
                if (grids[i][j] != grids[h1 + h2 - i][w1 + w2 - j]) {
                    return false;
                }
            }
        }
        return true;
    };

    int result = 0;
    for (int h1 = 0; h1 < h; h1++) {
        for (int w1 = 0; w1 < w; w1++) {
            for (int h2 = h1; h2 < h; h2++) {
                for (int w2 = w1; w2 < w; w2++) {
                   if (check(h1, w1, h2, w2)) result++;
                }
            }
        }
    }
    std::cout << result << std::endl;
    return 0;
}