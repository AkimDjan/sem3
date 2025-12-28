#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> gr; vector<int> used;

void DFS(int u) {
    used[u] = 1;
    for (int i=0; i<gr[u].size(); i++) {
        if (!used[gr[u][i]]) {
            DFS(gr[u][i]);
        }
    }
}

int main() {
    int v, e; cin >> v >> e;
    gr.resize(v);
    used.assign(v, 0);
    for (int i=0; i<e; i++) {
        int tmp1, tmp2; cin >> tmp1 >> tmp2;
        tmp1--; tmp2--;
        gr[tmp1].push_back(tmp2);
        gr[tmp2].push_back(tmp1);
    }
    if (e != v - 1) {
        cout << "not a tree";
        return 0;
    }
    DFS(0);
    for (int j=0; j<v; j++) {
        if (!used[j]) {
            cout << "not a tree";
            return 0;
        }
    }
    cout << "tree";
    return 0;
}
