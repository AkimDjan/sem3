#include <unordered_map>
#include <vector>
#include <queue>
#include <limits>

// еще раз про поиск путей
// в прошлый раз смотрели что есть алгоритм поиска пути от источника к стоку и наверное есть минимальное ребро,
// делаем обратные ребра на нашем графе, пропускную способность увеличиваем 
// на получившемся графе ищем путь, меняем веса по пути и делаем до тех пор когда не найдем путь 
// на прошлом занятии из dfs и bfs выбрали: лучше bfs 
// попробуем списком смежности

using Vertex = size_t;
using Graph = std::vector<std::unordered_map<Vertex, float>>;

float bfs(const Graph& gr, Vertex source, Vertex sink, std::vector<Vertex>& path) {
    std::vector<std::pair<Vertex, float>> used(gr.size(), {sink, -1});
    used[source] = {source, std::numeric_limits<float>::infinity()};
    std::queue<Vertex> q;
    q.push(source);
    while (q.size()) {
        Vertex front = q.front();
        q.pop();
        for (auto& a : gr[front]) {
            if (a.second > 0 && used[a.first].second == -1) { //a - adjacent
                q.push(a.first);
                std::pair cur = {front, std::min(used[front].second, a.second)};
                used[a.first] = cur;
                if (a.first == sink) {
                    auto cur = used[sink];
                    path.clear();
                    path.push_back(sink);
                    Vertex cur = sink;
                    while (cur != source) {
                        cur = used[cur].first;
                        path.push_back(cur);
                    }
                    path.push_back(source);
                    std::reverse(path.begin(), path.end());
                    return used[sink].second;
                }
            }

        }
    }
    return 0.0;
}

float maxFlow(Graph& gr, Vertex source, Vertex sink) {
    float flow = 0.0;
    std::vector<Vertex> path;
    while (true) {
        float add = bfs(gr, source, sink, path);
        if (add == 0.0) break;
        flow += add;
        for (size_t i = 0; i + 1 < path.size(); ++i) {
            Vertex u = path[i];
            Vertex v = path[i + 1];
            gr[u][v] -= add;
            gr[v][u] += add;
        }
    }

    return flow;
}