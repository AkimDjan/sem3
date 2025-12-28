// Дан направленный взвешенный граф - найти циклы с отрицательной суммой, если такие есть.
// Цикл с отрицательной суммой - это цикл в графе, сумма весов у которого отрицательная.

// Формат ввода
// В первой строке записано два натуральных числа через пробел V - количество вершин, E - количество ребер.
// В последующих E строках - три натуральных числа Si, Di, Wi через пробел, обозначающих направленное
// ребро и его вес.

// Формат вывода
// “No” - если циклов нет. “Yes” - если циклы есть.

// Пример
// Ввод
// 4 5
// 0 2 -2
// 1 0 4
// 1 2 -3
// 2 3 2
// 3 1 -1
// Вывод
// Yes

// Идея - Добавляем супер-вершину, соединённую со всеми вершинами нулевыми рёбрами, запускаем 
// алгоритм Беллмана–Форда и проверяем, возможна ли релаксация после V итераций — если да, в графе
// есть цикл с отрицательной суммой.

#include <iostream>
#include <vector>
using namespace std;

struct Edge {
    int u, v;
    long long w;
};

bool hasNegativeCycle(int V, vector<Edge> &edges) {
    const long long INF = 1e18;
    int super = V;
    vector<long long> dist(V + 1, INF);
    dist[super] = 0;
    for (int v = 0; v < V; ++v) {edges.push_back({super, v, 0});}
    for (int i = 0; i < V; ++i) {
        bool anyRelaxed = false;
        for (auto &e : edges) {
            if (dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
                anyRelaxed = true;
            }
        }
        if (!anyRelaxed) break;
    }

    for (auto &e : edges) {
        if (dist[e.u] + e.w < dist[e.v]) {return true;}
    }

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int V, E; if (!(cin >> V >> E)) return 0;
    vector<Edge> edges; edges.reserve(E);
    for (int i = 0; i < E; ++i) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }
    cout << (hasNegativeCycle(V, edges) ? "Yes\n" : "No\n");
    return 0;
}
