#include <iostream>
#include <vector>
#include <climits>
#include <stack>
using namespace std;

class Graph {
    int V;
    vector<vector<pair<int, int>>> adj;
public:
    Graph(int V): V(V), adj(V) {}
    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
    }
    void topologicalSortUtil(int v, vector<bool>& visited, stack<int>& Stack) {
        visited[v] = true;
        for (auto [neigh, w] : adj[v]) {
            if (!visited[neigh])
                topologicalSortUtil(neigh, visited, Stack);
        }
        Stack.push(v);
    }
    void longestPath(int S) {
        stack<int> Stack;
        vector<bool> visited(V, false);
        for (int i = 0; i < V; ++i)
            if (!visited[i])
                topologicalSortUtil(i, visited, Stack);
        vector<long long> dist(V, LLONG_MIN);
        dist[S] = 0;
        while (!Stack.empty()) {
            int u = Stack.top();
            Stack.pop();
            if (dist[u] != LLONG_MIN) {
                for (auto [v, w] : adj[u]) {
                    if (dist[v] < dist[u] + w)
                        dist[v] = dist[u] + w;
                }
            }
        }
        for (int i = 0; i < V; ++i) {
            if (dist[i] == LLONG_MIN)
                cout << "inf\n";
            else
                cout << dist[i] << "\n";
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int V, E, S; if (!(cin >> V >> E >> S)) return 0;
    Graph g(V);
    for (int i = 0; i < E; ++i) {
        int u, v, w; cin >> u >> v >> w;
        g.addEdge(u, v, w);
    }
    g.longestPath(S);
    return 0;
}
