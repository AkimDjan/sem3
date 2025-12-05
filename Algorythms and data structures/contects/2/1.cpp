#include <iostream>
#include <vector>
using namespace std;

struct Edge {
    int u, v;
    long long w;
};

bool hasNegativeCycle(int V, vector<Edge> &edges) {
    const long long INF = 1e18;
    int super = V;
    vector<long long> dist(V + 1, INF);
    dist[super] = 0;
    for (int v = 0; v < V; ++v) {edges.push_back({super, v, 0});}
    for (int i = 0; i < V; ++i) {
        bool anyRelaxed = false;
        for (auto &e : edges) {
            if (dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
                anyRelaxed = true;
            }
        }
        if (!anyRelaxed) break;
    }

    for (auto &e : edges) {
        if (dist[e.u] + e.w < dist[e.v]) {return true;}
    }

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int V, E; if (!(cin >> V >> E)) return 0;
    vector<Edge> edges; edges.reserve(E);
    for (int i = 0; i < E; ++i) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }
    cout << (hasNegativeCycle(V, edges) ? "Yes\n" : "No\n");
    return 0;
}
