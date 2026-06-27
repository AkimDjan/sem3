#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> gr; vector<int> tin, tout;
int timer = 0;

void DFS(int u, int parent) {
    tin[u] = ++timer;
    for (int v : gr[u]) {
        if (v != parent) {DFS(v, u);}
    }
    tout[u] = ++timer;
}

bool isAncestor(int u, int v) {
    return tin[u] <= tin[v] && tout[v] <= tout[u];
}

int main() {
    int n; cin >> n;
    gr.resize(n + 1);
    tin.resize(n + 1);
    tout.resize(n + 1);
    for (int i=0; i<n-1; i++) {
        int u, v; cin >> u >> v;
        gr[u].push_back(v);
        gr[v].push_back(u);
    }
    DFS(1, -1);
    int q; cin >> q;
    while (q--) {
        int u, v; cin >> u >> v;
        if (isAncestor(u, v)) {cout<<"YES"<<"\n";}
        else {cout<<"NO"<<"\n";}
    }
    return 0;
}
