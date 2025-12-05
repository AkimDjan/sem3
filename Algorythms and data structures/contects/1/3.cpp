
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int n, m;
vector<vector<int>> g; vector<bool> used;
vector<int> order;

void dfs(int v) {
    used[v] = true;
    order.push_back(v);
    for (int to : g[v]) {  
        if (!used[to]) {
            dfs(to);       
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    g.assign(n, {});
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (int i = 0; i < n; i++) {
        sort(g[i].begin(), g[i].end());
    }
    used.assign(n, false);
    for (int i = 0; i < n; i++) {
        if (!used[i]) {
            dfs(i);
            for (size_t i = 0; i < order.size(); i++) {
                cout << order[i] << " ";
            }
            cout << "\n";
        }
        order.clear();
    }
    cout << "\n";
}
