#include <queue>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::queue;
class Solution {
public:
    vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList) {
        wordList.insert(wordList.begin(), beginWord);
        int n = wordList.size();
        vector<vector<int>> edges(n);

        auto calc_distance = [&](string &s1, string &s2) -> int {
            int d = 0;
            for (int i = 0; i < s1.length(); i++) {
                if (s1[i] != s2[i]) {
                    d++;
                }
            }
            return d;
        };

        int end = -1;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int d = calc_distance(wordList[i], wordList[j]);
                if (d == 1) {
                    edges[i].push_back(j);
                    edges[j].push_back(i);
                }
            }
            if (calc_distance(wordList[i], endWord) == 0) end = i;
        }
        if (end == -1) return vector<vector<string>>();

        queue<int> q;
        q.push(0);
        vector<int> distance(n, 1e9);
        vector<bool> visited(n);
        visited[0] = true;
        distance[0] = 0;
        while (!q.empty()) {
            auto node = q.front();
            q.pop();
            if (distance[node] >= distance[end]) continue;
            for (auto next: edges[node]) {
                if (!visited[next]) {
                    visited[next] = true;
                    q.push(next);
                    distance[next] = distance[node] + 1;
                }
            }
        }

        vector<int> current_path;
        vector<vector<string>> ans;
        current_path.push_back(end);
        auto find_paths = [&](auto &self, int node) -> void {
            if (node == 0) {
                vector<string> final_path;
                for (int i = (int)current_path.size() - 1; i >= 0; i--) {
                    final_path.push_back(wordList[current_path[i]]);
                }
                ans.push_back(final_path);
                return;
            }
            for (auto next: edges[node]) {
                if (distance[next] != distance[node] - 1) continue;
                current_path.push_back(next);
                self(self, next);
                current_path.pop_back();
            }
        };
        find_paths(find_paths, end);
        return ans;
    }
};