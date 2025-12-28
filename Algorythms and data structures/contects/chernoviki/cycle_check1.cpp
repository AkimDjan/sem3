#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> gr; vector<bool> used;

bool DFS(int u, int parent) {
    used[u] = true;
    for (int v : gr[u]) {
        if (!used[v]) {if (DFS(v, u)) {return true;}}
        else if (v != parent) {return true;}
    }
    return false;
}

int main() {
    int v, e; cin >> v >> e;
    gr.resize(v);
    used.assign(v, false);
    for (int i=0; i<e; i++) {
        int tmp1, tmp2; cin >> tmp1 >> tmp2;
        tmp1--; tmp2--;
        gr[tmp1].push_back(tmp2);
        gr[tmp2].push_back(tmp1);
    }
    bool cycle = false;
    for (int i=0; i<v; i++) {
        if (!used[i]) {
            if (DFS(i, -1)) {
                cycle = true;
                break;
            }
        }
    }
    if (cycle) {cout<<"Graph contains a cycle"<<"\n";
    } else {cout << "Graph is acyclic\n";}
    return 0;
}
