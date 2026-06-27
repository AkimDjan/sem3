#include <iostream>
#include "graph.h"

using graph::Vertex;

int main() {
	graph::Graph gr(4);
	// graph::Verticies& adj =  gr.GetAdjacents(1);
	// adj.push_back(0);
	// adj.push_back(2);
	// adj.push_back(3);
	gr.AddEdge(0, 1);
	gr.AddEdge(0, 2);
	gr.AddEdge(1, 3);
	gr.AddEdge(2, 3);
	std::cout << "Graph gr";
	for (Vertex i = 0; i < 4; ++i) {
		std::cout << "\nadjacent for vertex " << i << ": ";
		for (Vertex v : gr.GetAdjacents(i))
			std::cout << v << " ";
	}
	graph::Graph g = std::move(gr);
	std::cout << "\ngraph gr is " << (gr.empty() ? "empty" : "not empty") << std::endl;
	std::cout << "Graph g";
	for (Vertex i = 0; i < 4; ++i) {
		std::cout << "\nadjacent for vertex " << i << ": ";
		for (Vertex v : g.GetAdjacents(i))
			std::cout << v << " ";
	}
	return 0;
}