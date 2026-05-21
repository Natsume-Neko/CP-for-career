#include <stack>
#include <vector>

int solution(std::vector<int> &A, std::vector<int> &B) {
    int n = A.size() + 1;
    std::vector<int> in_degree(n);
    std::vector<std::vector<int>> next(n);
    std::vector<bool> visited(n);
    for (int i = 0; i < n - 1; i++) {
        next[B[i]].push_back(A[i]); // reversed map
        in_degree[A[i]]++;
    }
    int root = -1;
    for (int i = 0; i < n; i++) {
        if (in_degree[i] == 0) {
            root = i;
            break;
        }
    }
    if (root == -1) return -1;
    std::stack<int> st;
    st.push(root);
    while (!st.empty()) {
        int cur = st.top();
        st.pop();
        if (visited[cur]) continue;
        visited[cur] = true;
        for (auto v: next[cur]) {
            if (!visited[v]) {
                st.push(v);
            }
        }
    } // To see whether it is a directed tree
    for (int i = 0; i < n; i++) {
        if (visited[i] != true) return -1;
    }
    return root;
}
