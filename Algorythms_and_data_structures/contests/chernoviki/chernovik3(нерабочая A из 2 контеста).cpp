

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;
    vector<int> tin, low;
    vector<pair<int, int>> bridges;
    int timer;
    void dfs(int v, int p = -1) {
        tin[v] = low[v] = timer++;
        for (int to : adj[v]) {
            if (to == p) continue;
            if (tin[to] != -1) {
                low[v] = min(low[v], tin[to]);
            } else {
                dfs(to, v);
                low[v] = min(low[v], low[to]);
                if (low[to] > tin[v]) {
                    int a = min(v, to);
                    int b = max(v, to);
                    bridges.push_back({a, b});
                }
            }
        }
    }
public:
    Graph(int V): V(V), adj(V) {}
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    void findBridges() {
        tin.assign(V, -1);
        low.assign(V, -1);
        timer = 0;
        bridges.clear();
        dfs(0);
        if (bridges.empty()) {
            cout << "Yes\n";
        } else {
            sort(bridges.begin(), bridges.end(), [](auto &a, auto &b) {
                if (a.first + a.second == b.first + b.second)
                    return a.first < b.first;
                return a.first + a.second < b.first + b.second;
            });
            cout << bridges.size() << "\n";
            for (auto &e : bridges)
                cout << e.first << " " << e.second << "\n";
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int V, E; if (!(cin >> V >> E)) return 0;
    Graph g(V);
    for (int i = 0; i < E; ++i) {
        int u, v; cin >> u >> v;
        g.addEdge(u, v);
    }
    g.findBridges();
    return 0;
}
