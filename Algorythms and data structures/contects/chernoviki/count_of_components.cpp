#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> gr;
vector<int> used;
int k = 1;

void DFS(int u, int number) {
    used[u] = number;
    cout<<u<<" ";
    for (int i = 0; i < gr[u].size(); i++) {
        if (!used[gr[u][i]]) {
            DFS(gr[u][i], number);
        }
    }
}

int main() {
    int v, e; cin >> v >> e;
    gr.resize(v); used.assign(v, 0);
    for (int i = 0; i < e; i++) {
        int tmp1, tmp2; cin >> tmp1 >> tmp2;
        tmp1--; tmp2--;
        gr[tmp1].push_back(tmp2);
        gr[tmp2].push_back(tmp1);
    }
    for (auto vec : gr) {sort(vec.begin(),vec.end());}
    for (int i = 0; i < v; i++) {
        if (!used[i]) {
            DFS(i, k);
            k++;
        }
    }
    vector<vector<int>> components(k);
    for (int i = 0; i < v; i++) {components[used[i]].push_back(i + 1);}
    for (int i = 1; i < k; i++) {
        cout << i << " " << components[i].size() << "\n";
        for (int tmp : components[i]) {
            cout << tmp << " ";
        }
        cout << "\n";
    }
    return 0;
}
