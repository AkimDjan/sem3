// Двудольный граф или биграф в теории графов - это граф, вершины которого можно 
// разбить на две части так, что каждое ребро соединяет вершину из одной части с вершиной
// другой части. То есть, между вершинами одной и той же части рёбра отсутствуют.

// Формат ввода
// В первой строке записано два натуральных числа через пробле V - количество вершин, E - количество ребер.
// В последующих E строках - два натуральных числа Si, Di через пробел, обозначающих не направленое ребро.

// Формат вывода
// "Graph is bipartite", если граф Двудольный.
// "Graph is not bipartite", если граф не Двудольный.

// Пример 
// Ввод
// 9 8
// 0 1
// 1 2
// 1 7
// 2 3
// 3 5
// 4 6
// 4 8
// 7 8
// Вывод
// Graph is bipartite

// Идея - раскраска вершин. Граф двудольный, если любые 2 соседние вершины не одного цвета при покраске
// (все ребра соединяют вершины разных цветов)

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
