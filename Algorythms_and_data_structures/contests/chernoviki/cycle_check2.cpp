#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> gr;
vector<int> used;

bool DFS(int u) {
    used[u] = 1;
    for (int v : gr[u]) {
        if (used[v] == 0) {
            if (DFS(v)) return true;
        } else if (used[v] == 1) {
            return true;
        }
    }
    used[u] = 2;
    return false;
}

int main() {
    int v, e;
    cin >> v >> e;

    gr.resize(v);
    used.assign(v, 0);

    for (int i = 0; i < e; i++) {
        int tmp1, tmp2;
        cin >> tmp1 >> tmp2;
        tmp1--; tmp2--;
        gr[tmp1].push_back(tmp2);
    }

    bool cycle = false;
    for (int i = 0; i < v; i++) {
        if (used[i] == 0) {
            if (DFS(i)) {
                cycle = true;
                break;
            }
        }
    }

    if (cycle) {
        cout << "Graph contains a cycle\n";
    } else {
        cout << "Graph is acyclic\n";
    }

    return 0;
}
