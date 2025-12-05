#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include <stdexcept>
#include <string>
	
namespace graph {
using Vertex = size_t;
using Verticies = std::vector<Vertex>;

class Graph{
public:
    Graph() : graph_() {}
    Graph(size_t vertex_count) : graph_(vertex_count) {}
    Graph(const Graph& rhs) : graph_(rhs.graph_) {}
    Graph(Graph&& rhs) noexcept : graph_(std::move(rhs.graph_)) {}
    size_t size() const { return graph_.size(); }
    bool empty() const { return !size(); }
    void resize(size_t new_vertex_count) {
        graph_.clear();
        graph_.resize(new_vertex_count);
    }
    const std::vector<Vertex>& GetAdjacents(Vertex v) const {return graph_.at(v);}
    // std::vector<Vertex>& GetAdjacents(Vertex v) {
    //     return graph_.at(v); Присваивая сюда вектор никаких проверок не делаем
    // }
    void AddEdge(Vertex from, Vertex to) {
        if(to >= graph_.size()) {
            std::string error = "There_is no vertex with index " + std::to_string(to);
            throw std::logic_error(error);
        }
        graph_.at(from).push_back(to);
    }
private:
    std::vector<std::vector<Vertex>> graph_;
};
}
#endif // _GRAPH_H_