// Дан направленный граф, проверить является ли граф "сильно" связным.
// Ориентированный граф называется сильно-связным, если в нём существует (ориентированный) 
// путь из любой вершины в любую другую, или, что эквивалентно, граф содержит ровно 
// одну сильно связную компоненту.

// Формат ввода
// В первой строке записано два натуральных числа через пробел V - количество вершин, E - количество ребер.
// В последующих E строках - два натуральных числа Si, Di через пробел, обозначающих направленное ребро.

// Формат вывода
// Yes - Если граф "сильно" связный, иначе No.

// Пример
// Ввод
// 5 8
// 0 4
// 1 0
// 1 2
// 2 1
// 2 4
// 3 1
// 3 2
// 4 3
// Вывод
// Yes

// Идея - упрощенный Алгоритм Косарайю, в данном случае нам просто нужно убедиться что при запуске на графе
// DFS и на транспонированном (revAdj) графе мы из 0 можем всюду добраться и в 0 также можем добраться =>
// из любой вершины можно попасть в любую другую!

#include <iostream>
#include <vector>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;
    vector<vector<int>> revAdj;
public:
    Graph(int V): V(V), adj(V), revAdj(V) {}
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        revAdj[v].push_back(u);
    }
    void dfs(int v, vector<bool>& visited, const vector<vector<int>>& graph) {
        visited[v] = true;
        for (int neigh : graph[v]) {
            if (!visited[neigh])
                dfs(neigh, visited, graph);
        }
    }
    bool isStronglyConnected() {
        vector<bool> visited(V, false);
        dfs(0, visited, adj);
        for (bool v : visited) {if (!v) {return false;}}
        fill(visited.begin(), visited.end(), false);
        dfs(0, visited, revAdj);
        for (bool v : visited) {if (!v) {return false;}}
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int V, E; if (!(cin >> V >> E)) return 0;
    Graph g(V);
    for (int i = 0; i < E; ++i) {
        int u, v; cin >> u >> v;
        g.addEdge(u, v);
    }
    cout << (g.isStronglyConnected() ? "Yes\n" : "No\n");
    return 0;
}