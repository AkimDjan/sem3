#include <iostream>
#include <vector>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;
public:
    Graph(int V): V(V), adj(V) {}
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    bool isCyclicUtil(int v, int parent, vector<bool>& visited) {
        visited[v] = true;
        for (int neigh : adj[v]) {
            if (!visited[neigh]) {
                if (isCyclicUtil(neigh, v, visited)) return true;
            } else if (neigh != parent) {
                return true;
            }
        }
        return false;
    }
    bool isConnected(vector<bool>& visited) {
        for (bool v : visited)
            if (!v) return false;
        return true;
    }
    bool isTree() {
        vector<bool> visited(V, false);
        if (isCyclicUtil(0, -1, visited)) return false;
        if (!isConnected(visited)) return false;
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int V, E;
    if (!(cin >> V >> E)) return 0;
    Graph g(V);
    for (int i = 0; i < E; ++i) {
        int u, v; cin >> u >> v;
        g.addEdge(u, v);
    }
    cout << (g.isTree() ? "Yes\n" : "No\n");
    return 0;
}
