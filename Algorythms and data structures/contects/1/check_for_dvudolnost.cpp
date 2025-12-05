#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> gr; vector<int> color;

bool DFS(int u, int c) {
    color[u] = c;
    for (int v : gr[u]) {
        if (color[v] == -1) {if (!DFS(v, 1 - c)) {return false;}}
        else if (color[v] == color[u]) {return false;}
    }
    return true;
}

int main() {
    int v, e; cin >> v >> e;
    gr.resize(v); color.assign(v, -1);
    for (int i=0; i<e; i++) {
        int u, w; cin >> u >> w;
        gr[u].push_back(w); gr[w].push_back(u); 
    }
    bool bipartite = true;
    for (int i=0; i<v; i++) {
        if (color[i] == -1) {
            if (!DFS(i, 0)) {
                bipartite = false;
                break;
            }
        }
    }
    if (bipartite) {cout<<"Graph is bipartite"<<"\n";} 
    else {cout<<"Graph is not bipartite"<<"\n";}
    return 0;
}
