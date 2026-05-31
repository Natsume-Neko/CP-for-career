#include <iostream>
#include <utility>
#include <vector>
int main() {
    int t;
    std::cin >> t;
    while (t--) {
        std::string s;
        std::cin >> s;
        std::string ans;
        std::vector<int> cnt(26);
        for (auto c: s) {
            cnt[c - 'a']++;
        }
        auto max_cnt_without_c = [&](char c) {
            c -= 'a';
            int max_num = 0;
            char max_char;
            for (int i = 0; i < 26; i++) {
                if (i != c) {
                    if (cnt[i] > max_num) {
                        max_num = cnt[i];
                        max_char = i;
                    } 
                }
            }
            max_char += 'a';
            return std::make_pair(max_num, max_char);
        };
        if (max_cnt_without_c('z' + 1).first > (s.length() + 1) / 2) {
            std::cout << "No\n";
            continue;
        }
        char lst = 'z' + 1;
        while (ans.length() != s.length()) {
            char c = max_cnt_without_c(lst).second;
            ans.push_back(c);
            cnt[c - 'a']--;
            lst = c;
        }
        ans.push_back('\n');
        std::cout << "Yes\n" << ans;
    }
    return 0;
}