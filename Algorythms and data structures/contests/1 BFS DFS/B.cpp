// Используя список смежности, полученный в прошлой задаче отобразите "историю обхода 
// в глубину" для заданного графа.

// Формат ввода
// В первой строке записано два натуральных числа через пробле V - количество вершин, E - количество ребер.
// В последующих E строках - два натуральных числа Si, Di через пробел, обозначающих не направленое ребро.

// Формат вывода
// Номера вершин в порядке обхода, начиная с первой (1) вершины.

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
// 1 2 5 9 10 6 3 4 7 11 12 8

// Идея - классический обход в глубину, пока у нас не закончатся соседи, то идем глубже и глубже

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