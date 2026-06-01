#include <array>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct Position {
    int x;
    int y;
    bool is_even;
};

int main() {
    std::array<int, 8> directions_x{
        1, 1, 0, 0, -1, -1, 1, -1
    };
    std::array<int, 8> directions_y{
        1, -1, 1, -1, 1, -1, 0, 0
    };
    int h, w;
    std::cin >> h >> w;
    std::vector<std::string> board(h);
    std::vector<std::vector<bool>> is_even(h, std::vector<bool>(w)), visited(h, std::vector<bool>(w));
    for (int i = 0; i < h; i++) {
        std::cin >> board[i];
    }
    std::queue<Position> q;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (board[i][j] == '#') {
                bool is_valid = false;
                for (int k = 0; k < 8; k++) {
                    int next_x = directions_x[k] + i;
                    int next_y = directions_y[k] + j;
                    if (next_x >= 0 && next_x < h && next_y >= 0 && next_y < w) {
                        if (board[next_x][next_y] == '.') {
                            is_valid = true;
                            break;
                        }
                    }
                }
                if (is_valid) {
                    is_even[i][j] = true;
                    q.push(Position {i, j, true});
                    visited[i][j] = true;
                }
            }
        }
    }
    while (!q.empty()) {
        auto pos = q.front();
        q.pop();
        for (int i = 0; i < 8; i++) {
            int next_x = directions_x[i] + pos.x;
            int next_y = directions_y[i] + pos.y;
            if (next_x >= 0 && next_x < h && next_y >= 0 && next_y < w && !visited[next_x][next_y]) {
                q.push(Position {next_x, next_y, !pos.is_even});
                visited[next_x][next_y] = true;
                is_even[next_x][next_y] = !pos.is_even;
            }
        }
    }
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (is_even[i][j]) {
                std::cout << '#';
            } else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
    return 0;
}