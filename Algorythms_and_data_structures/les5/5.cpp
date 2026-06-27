#include <iostream>
#include <vector>
#include <bits/stdc++.h>

// Задача о непрерывном рюкзаке ... чекрыжим и все норм
// если нельзя чекрыжить - тогда ну жадный шаг не работает, поэтому внимательно короче

// графы, долги разобрали с того семестра, теперь идем дальше
// Опр. Граф - множество вершин , множество ребер, и неупорядоченная пара (граф), если упор. - то 
// граф (ну любой) | Абстракция        | Реализация
//                   матрица смежности
//                   матрица инцидентности
//                   списки смежности

// colors [|u|]
// for u принадл U
//dfs(u) {
// if (colors[u]==white){
//      colors[u]=gray;
//      обработка
//      идем по смежным вершинам
//      dfs(u)
//      обработки(u)
//      colors[u]=black;
//}}
// не можем детектировать цикл только тогда когда граф ромбовидный из вершины выходят 2 ориентированные
// а из этих 2 вершин тоже в одну другую выходят 
// можно типо стек добавить чтобы детектировать такие ужасные случаи
// 
// for u принадл U
// bfs(u){ 
// if(colors[u]==white){
//      colors[u]=black;
//      queue.push(u);
//      while(!queue.empty()){
//      бла бла бла
//}}
// задачко 
//
using vertex = size_t;
class Graph{
    public:
        class Graph();
        bool empty() {return !size();}
        size_t size() const {return graph_.size();}
        const std::vector<vertex>& GetAdjacents(vertex v) const;
        const std::vector<vertex>& GetAdjacents(vertex v);
        Graph(const Graph& rhs) {graph_=rhs.graph_;}
        Graph(Graph&& rhs);
    private:
        std::vector<std::vector<vertex>> graph_;
};