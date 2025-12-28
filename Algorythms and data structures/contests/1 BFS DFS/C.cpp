// Используя список смежности, полученный в первой задаче, и поиск в глубину, 
// полученный во второй, отобразите "историю обхода в глубину" для каждого компонента заданного графа.

// Формат ввода
// В первой строке записано два натуральных числа через пробле V - количество вершин, E - количество ребер.
// В последующих E строках - два натуральных числа Si, Di через пробел, обозначающих не направленое ребро.

// Формат вывода
// Номера вершин в порядке обхода, начиная с нулевой (0) вершины.

// Пример
// Ввод
// 15 11
// 1 3
// 1 2
// 1 4
// 2 5
// 2 6
// 5 9
// 5 10
// 4 7
// 4 8
// 7 11
// 7 12
// Вывод
// 0 
// 1 2 5 9 10 6 3 4 7 11 12 8 
// 13 
// 14

// Идея - по сути количество компонент связности и есть количество DFS запущенных на графе. 
// Следовательно сначала дфс а потом обработка

#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int n, m;
vector<vector<int>> g; vector<bool> used;
vector<int> order;

void dfs(int v) {
    used[v] = true;
    order.push_back(v);
    for (int to : g[v]) {  
        if (!used[to]) {
            dfs(to);       
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    g.assign(n, {});
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (int i = 0; i < n; i++) {
        sort(g[i].begin(), g[i].end());
    }
    used.assign(n, false);
    for (int i = 0; i < n; i++) {
        if (!used[i]) {
            dfs(i);
            for (size_t i = 0; i < order.size(); i++) {
                cout << order[i] << " ";
            }
            cout << "\n";
        }
        order.clear();
    }
    cout << "\n";
}
