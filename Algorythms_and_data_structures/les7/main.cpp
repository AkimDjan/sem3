#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits>
#include <algorithm>

template <class T, class HASH, class EQUAL>
class Heap {
public:
    Heap(HASH hash, EQUAL equal) : elem_to_index_(10, hash, equal) {}   // 10 is initial bucket size here
    size_t size() {
        return heap_.size();
    }
    T top() {
        return heap_[0];
    }
    void pop() {
        elem_to_index_.erase(heap_[0]);
        std::swap(heap_[0], heap_[heap_.size() - 1]);
        elem_to_index_[heap_[0]] = 0;
        heap_.pop_back();
        Down(0);
    }
    void push(const T& element) {
        auto new_ind = heap_.size();
        heap_.push_back(element);
        elem_to_index_[element] = new_ind;
        Up(new_ind);
    }
    void swap(T old_el, T new_el) {
        //must be check if old_el is in elem_to_index_, but I'm lazy
        auto index = elem_to_index_[old_el];
        if (new_el < old_el) {
            elem_to_index_.erase(old_el);
            elem_to_index_[new_el] = index;
            heap_[index] = new_el;
            Up(index);
        } else if (old_el < new_el) {
            elem_to_index_.erase(old_el);
            elem_to_index_[new_el] = index;
            heap_[index] = new_el;
            Down(index);
        }
    }
private:
    std::vector<T> heap_;
    std::unordered_map<T, size_t, HASH, EQUAL> elem_to_index_;
    size_t Parent(size_t index) {
        return (index - 1) >> 1;
    }
    size_t Left(size_t index) {
        return (index << 1) + 1;
    }
    size_t Right(size_t index) {
        return (index + 1) << 1;
    }
    void Up(size_t index) {
        auto parent = Parent(index);
        while (index > 0 && heap_[index] < heap_[parent]) {
            elem_to_index_[heap_[index]] = parent;
            elem_to_index_[heap_[parent]] = index;
            std::swap(heap_[index], heap_[parent]);
            index = parent;
            parent = Parent(index);
        }
    }
    void Down(size_t index) {
        auto left = Left(index);
        auto right = Right(index);
        while (left < heap_.size()) {
            auto index_for_swap = index;
            if (right < heap_.size() && heap_[right] < heap_[left]) {
                if (heap_[right] < heap_[index]) {
                    index_for_swap = right;
                }
            } else if (heap_[left] < heap_[index]) {
                index_for_swap = left;
            }

            if (index == index_for_swap)
                break;
            else {
                elem_to_index_[heap_[index]] = index_for_swap;
                elem_to_index_[heap_[index_for_swap]] = index;
                std::swap(heap_[index], heap_[index_for_swap]);
                index = index_for_swap;
                left = Left(index);
                right = Right(index);
            }
        }
    }
};

using Vertex = size_t;

struct DistanceToVertex {
    Vertex v;
    double distance = 0;
    bool operator<(DistanceToVertex rhs) {
        return distance < rhs.distance;
    }
};

using Adjacents = std::vector<DistanceToVertex>;
using Graph = std::vector<Adjacents>;

std::vector<Vertex> GetPath(const Graph& g, Vertex from, Vertex to) {
    std::vector<Vertex> prev(g.size());
    std::vector<double> dist(g.size(), std::numeric_limits<double>::max());
    //std::vector<bool> used(g.size(), false);
    auto hash = [](DistanceToVertex d) {return std::hash<size_t>()(d.v); };
    auto equal = [](DistanceToVertex lhs, DistanceToVertex rhs) {return lhs.v == rhs.v; };
    Heap < DistanceToVertex, decltype(hash), decltype(equal)> pq(hash, equal);
    pq.push({ from, 0 });
    dist[from] = 0;
    while (pq.size()) {
        auto top = pq.top();
        pq.pop();
        if (top.v == to)
            break;
        //used[top.v] = true;
        for (auto e : g[top.v]) {
            auto distance = top.distance + e.distance;
            if (distance < dist[e.v]) {
                if (dist[e.v] == std::numeric_limits<double>::max())
                    pq.push({ e.v, distance });
                else
                    pq.swap({ e.v, dist[e.v] }, { e.v, distance });
                dist[e.v] = distance;
                prev[e.v] = top.v;
            }
        }
    }
    if (dist[to] == std::numeric_limits<double>::max())
        return {};
    std::vector<Vertex> res;
    Vertex cur = to;
    res.push_back(to);
    while (cur != from) {
        cur = prev[cur];
        res.push_back(cur);
    }
    std::reverse(res.begin(), res.end());
    return res;
}

int main() {

    Graph g = {
        {{1, 2}, {2, 7}, {3, 9}},
        {{0, 2}, { 2, 1 }, {4, 1}},
        {{0, 7}, {1, 1}, {3, 5}, {4, 2}, {5, 5}},
        {{0, 9}, {2, 5}, {5, 2}},
        {{1, 1}, {2, 2}, {5, 3}},
        {{2, 5}, {3, 2}, {4, 3}, {6, 1}},
        {{5, 1}}
    };
    for (auto v : GetPath(g, 0, 6))
        std::cout << v << " " << std::endl;
    return 0;
}