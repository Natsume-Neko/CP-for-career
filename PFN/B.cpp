#include <algorithm>
#include <iostream>
#include <vector>
int main() {
    int N, K;
    double sigma;
    std::cin >> N >> K >> sigma;
    std::vector<double> A(N), mu(K);
    for (int i = 0; i < N; i++) {
        std::cin >> A[i];
    }
    for (int i = 0; i < K; i++) {
        std::cin >> mu[i];
    }

    auto search_start = mu.begin();
    ++search_start; // For binary search of mu

    std::vector<double> suffix(N + 1), suffix_square_sum(N + 1); // Suffix sum and squared sum
    for (int i = N - 1; i >= 0; i--) {
        suffix[i] = suffix[i + 1] + A[i];
        suffix_square_sum[i] = suffix_square_sum[i + 1] + A[i] * A[i];
    }

    double min_like = -1, prefix_dif_sum = 0;
    int ans_t = -1, ans_mu = -1;
    for (int t = 1; t < N; t++) {
        double suffix_mean = suffix[t] / (N - t);
        int pos = std::lower_bound(search_start, mu.end(), suffix_mean) - mu.begin();
        if (pos == K) {
            pos -= 1;
        } else if (pos != 1) {
            if (mu[pos] - suffix_mean > suffix_mean - mu[pos - 1]) {
                pos -= 1;
            }
        } // If t is determined, then the best mu is the one most close to the mean of the sequence

        double like = prefix_dif_sum + suffix_square_sum[t] + suffix_mean * (suffix_mean * (N - t) - 2 * suffix[t]) + (N - t) * (suffix_mean - mu[pos]) * (suffix_mean - mu[pos]);
        // Calculate the log of the likelihood (ignore the constants because they do not influence the answer)
        // And negate them for convenience, so we get the minimum of them
        if (t == 1 || min_like > like) {
            min_like = like;
            ans_t = t + 1;
            ans_mu = pos + 1;
        }
        prefix_dif_sum += (A[t] - mu[0]) * (A[t] - mu[0]);
    }
    std::cout << ans_t << " " << ans_mu << std::endl;
    return 0;
}