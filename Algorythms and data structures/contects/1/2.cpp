#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> gr;  vector<int> used;


void DFS(int u) {
    used[u] = 1;
    for (int i=0; i<gr[u].size(); i++) {
        if (!used[gr[u][i]]) {
            DFS(gr[u][i]);
        }
    }
}

int main() {
    int v, e; cin>>v>>e;
    gr.resize(v);
    for (int i=0; i<e; i++) {
        int tmp1, tmp2; cin >> tmp1 >> tmp2;
        gr[tmp1].push_back(tmp2);
        gr[tmp2].push_back(tmp1);
    }
    for (auto vec : gr) {
        sort(vec.begin(),vec.end());
        if (vec.empty()) {
                cout<<"-"<<"\n";
                continue;
            }
        for (int i=0; i<vec.size(); i++) {
            cout << vec[i] << " ";
            
        } 
        cout << "\n";
    }
    return 0;
}





