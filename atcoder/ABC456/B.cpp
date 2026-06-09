#include <iostream>
#include <vector>
int main() {
    std::vector<std::vector<int>> dices(3, std::vector<int>(3));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 6; j++) {
            std::cin >> dices[i][j];
        }
    }
    double ans = 0;
    for (int i = 4; i <= 6; i++) {
        for (int j = 4; j <= 6; j++) {
            for (int k = 4; k <= 6; k++) {
                if (i != j && i != k && j != k) {
                    double p = 1;
                    int cnt = 0;
                    for (int l = 0; l < 6; l++) {
                        if (dices[0][l] == i) {
                            cnt++;
                        }
                    }
                    p *= cnt / 6.0;
                    cnt = 0;
                    for (int l = 0; l < 6; l++) {
                        if (dices[1][l] == j) {
                            cnt++;
                        }
                    }
                    p *= cnt / 6.0;
                    cnt = 0;
                    for (int l = 0; l < 6; l++) {
                        if (dices[2][l] == k) {
                            cnt++;
                        }
                    }
                    p *= cnt / 6.0;
                    ans += p;
                }
            }
        }
    }
    std::cout << ans << std::endl;
    return 0;
}