// Дан взвешенный направленный ацикличный граф и начальная вершина, найти сумму весов самого длинного
// пути из начальной вершины до всех остальных вершин графа, если вершина недостижима из данной
// начальной точки, написать длину до вершины, как inf.

// Задача обратная кратчайшим путям из заданной вершины - необходимо найти самые длинные пути из
// заданной вершины.

// Формат ввода
// В первой строке записано три натуральных числа через пробел V - количество вершин, E - количество
// ребер, S - начальная вершина.
// В последующих E строках - три натуральных числа Si, Di, Wi через пробел, 
// обозначающих направленное ребро и его вес.

// Формат вывода
// V строк, где номер строки соответствует номеру вершины, а в строке указана сумма самого
// длинного пути до вершины, если вершина недостижима из данной начальной точки необходимо вывести inf.

// Пример
// Ввод
// 8 11 1
// 0 6 2
// 1 2 -4
// 1 4 1
// 1 6 8
// 3 0 3
// 3 4 5
// 5 1 2
// 7 0 6
// 7 1 -1
// 7 3 4
// 7 5 -4
// Вывод
// inf
// 0
// -4
// inf
// 1
// inf
// 8
// inf

// Идея - Топологическая сортировка, после этого динамически считаем длинные пути через релаксацию
// в другую сторону

#include <iostream>
#include <vector>
#include <climits>
#include <stack>
using namespace std;

class Graph {
    int V;
    vector<vector<pair<int, int>>> adj;
public:
    Graph(int V): V(V), adj(V) {}
    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
    }
    void topologicalSortUtil(int v, vector<bool>& visited, stack<int>& Stack) {
        visited[v] = true;
        for (auto [neigh, w] : adj[v]) {
            if (!visited[neigh])
                topologicalSortUtil(neigh, visited, Stack);
        }
        Stack.push(v);
    }
    void longestPath(int S) {
        stack<int> Stack;
        vector<bool> visited(V, false);
        for (int i = 0; i < V; ++i)
            if (!visited[i])
                topologicalSortUtil(i, visited, Stack);
        vector<long long> dist(V, LLONG_MIN);
        dist[S] = 0;
        while (!Stack.empty()) {
            int u = Stack.top();
            Stack.pop();
            if (dist[u] != LLONG_MIN) {
                for (auto [v, w] : adj[u]) {
                    if (dist[v] < dist[u] + w)
                        dist[v] = dist[u] + w;
                }
            }
        }
        for (int i = 0; i < V; ++i) {
            if (dist[i] == LLONG_MIN)
                cout << "inf\n";
            else
                cout << dist[i] << "\n";
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int V, E, S; if (!(cin >> V >> E >> S)) return 0;
    Graph g(V);
    for (int i = 0; i < E; ++i) {
        int u, v, w; cin >> u >> v >> w;
        g.addEdge(u, v, w);
    }
    g.longestPath(S);
    return 0;
}
