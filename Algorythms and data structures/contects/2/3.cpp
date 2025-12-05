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
    }
    bool isCyclicUtil(int v, vector<bool>& visited, vector<bool>& recStack) {
        visited[v] = true; recStack[v] = true;
        for (int neigh : adj[v]) {
            if (!visited[neigh] && isCyclicUtil(neigh, visited, recStack))
                return true;
            else if (recStack[neigh])
                return true;
        }
        recStack[v] = false;
        return false;
    }
    bool isDAG() {
        vector<bool> visited(V, false);
        vector<bool> recStack(V, false);
        for (int i = 0; i < V; ++i) {
            if (!visited[i] && isCyclicUtil(i, visited, recStack))
                return false;
        }
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int V, E; if (!(cin >> V >> E)) return 0;
    Graph g(V);
    for (int i = 0; i < E; ++i) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }
    cout << (g.isDAG() ? "Yes\n" : "No\n");
    return 0;
}
