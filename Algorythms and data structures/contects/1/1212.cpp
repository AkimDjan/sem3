#include <algorithm>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> gr;
vector<bool> used;

void DFS(int u) {
    used[u] = true; cout << u << " ";
    for (int i=0; i < gr[u].size(); i++) {
        int n = gr[u][i];
        if (!used[n]) {DFS(n);}
    }
}

int main() {
    int v, e; cin >> v >> e;
    gr.resize(v+1); used.assign(v+1, false);
    for (int i=0; i<e; i++) {
        int tmp1, tmp2; cin >> tmp1 >> tmp2;
        gr[tmp1].push_back(tmp2);
        gr[tmp2].push_back(tmp1);
    }
    for (int i = 1; i <= v; i++) {sort(gr[i].begin(), gr[i].end());}
    DFS(1);
}