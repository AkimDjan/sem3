#include <iostream>
#include <limits>
#include "graph.h"
#include <algorithm>

using Vertex = size_t;
// остановились на поиске путей, волновой алгоритм по сути BFS, шли из какой-то вершины, ставили метки
// инвертируем граф и идем от больших меток последовательно к меньшим
// когда взвесили граф - Дейкстра?
// по сути так же волнами действуем и создаем там где надо фиктивные вершины. Но как действовать с 1.5 весом?
// тогда очередь меняем на очередь с приоритетом
// заметаем только те вершинки до которых мы дошли 
// приоритет - длина текущего пути, а в очереди будут вершинки до которых мы дошли
// Мы можем на самом деле уже записать псведокод

// веса можно считать как стоимость прохода и алгоритм дейкстры будет работать 
// a ->(1) b
//   \(2)  |(0.5)
//      c
// закинули с(2) или c(1.5) 
// тут будет наш вектор - аки хеш таблица
// будем еще хранить вектор с краткими путями и делать heapify 
// восстанавливаем путь 

int Inf = std::numeric_limits<size_t>::max();

struct Edge {
    Vertex to;
    float distance = 0;
    bool operator<(const Edge& rhs) {return this->distance < rhs.distance;}
}; // можно и парой но лучше структуркой - можно называть 

using Graph = std::vector<std::vector<Edge>>;

template <class T>
struct Heap{
    void push(T value);
    void pop();
    T top();
    bool empty();
    void swap(T rhs, T lhs);
};

std::vector<Vertex> DijkstraAlgorythm(const Graph& g, Vertex from, Vertex to) {
    Heap<Edge> pq;
    std::vector<Vertex> backtrack(g.size());
    std::vector<float> cur_dist(g.size(), Inf);
    pq.push({from,0});
    while(!pq.empty()){
        Edge cur = pq.top();
        if (cur.to==to) {break;}
        for ( Edge e: g[cur.to]) {
            if (cur.distance+e.distance<cur_dist[e.to]) {
                if (cur_dist[e.to]==Inf) {
                    pq.push({e.to, cur.distance+e.distance});
                } else {
                    pq.swap({e.to, cur_dist[e.to]},{e.to, cur.distance +e.distance});
                }
                cur_dist[e.to]=e.to, cur.distance +e.distance;
                backtrack[e.to]=cur.to;
            }
        }
        pq.pop();
    }    
    if (cur_dist[to]==Inf) {
        return {};
    }
    Vertex cur_back = to;
    std::vector<Vertex> res;
    while (cur_back!=from) {
        res.push_back(cur_back);
        cur_back=backtrack[cur_back];
    }
    res.push_back(from);
    std::reverse(res.begin(), res.end());
    return res;
}
