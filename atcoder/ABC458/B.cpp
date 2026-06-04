#include <iostream>
int main() {
    int h, w;
    std::cin >> h >> w;
    if (h == 1 && w == 1) {
        std::cout << "0" << std::endl;
        return 0;
    }
    if (h == 1) {
        for (int j = 0; j < w; j++) {
            if (j == 0 || j == w - 1) {
                std::cout << "1 ";
            } else {
                std::cout << "2 ";
            }
        }
        std::cout << std::endl;
        return 0;
    }
    if (w == 1) {
        for (int i = 0; i < h; i++) {
            if (i == 0 || i == h - 1) {
                std::cout << "1\n";
            } else {
                std::cout << "2\n";
            }
        }
        return 0;
    }
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if ((i == 0 || i == h - 1) && (j == 0 || j == w - 1)) {
                std::cout << "2 ";
            } else if (i == 0 || i == h - 1 || j == 0 || j == w - 1) {
                std::cout << "3 ";
            } else {
                std::cout << "4 ";
            }
        }
        std::cout << std::endl;
    }
    return 0;
}