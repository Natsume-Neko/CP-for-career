#include <array>
#include <iostream>

int main() {
    std::array<std::array<char, 50>, 50> dungeon;
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j += 2) {
            dungeon[i][j] = '.';
        }
        for (int j = 1; j < 50; j += 2) {
            dungeon[i][j] = '#';
        }
    }
    for (int j = 3; j < 50; j += 4) {
        dungeon[0][j] = '.';
    }
    for (int j = 1; j < 50; j += 4) {
        dungeon[49][j] = '.';
    }
    dungeon[0][0] = 'S'; // Start Point is always (0, 0)
    
    int k;
    std::cin >> k;
    int col = k / 51;
    if ((col & 1) == 0) { // Check if the "column" is odd or even
        if ((k + 1) % 51 == 0) { // Check if the goal is in corner
            dungeon[49][col * 2 + 1] = 'G';
        } else {
            dungeon[(k + 1) % 51 - 1][col * 2] = 'G'; // From top to bottom
        }
    } else {
        if ((k + 1) % 51 == 0) {
            dungeon[0][col * 2 + 1] = 'G';
        } else {
            dungeon[50 - (k + 1) % 51][col * 2] = 'G'; // From bottom to top
        }
    }

    std::cout << "50 50\n";
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            std::cout << dungeon[i][j];
        }
        std::cout << '\n';
    }
    return 0;
}