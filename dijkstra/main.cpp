//
// Created by aolo2 on 7/10/17.
//

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <map>
#include <unordered_map>

#include "min_pqueue.hpp"

struct vertex {
    unsigned long id;
    float dist;

    bool operator>(const vertex &other) const { return dist > other.dist; }

    bool operator<(const vertex &other) const { return dist < other.dist; }

    bool operator==(const vertex &other) const { return id == other.id; }
};

struct edge {
    vertex target;
    float weight;           // mb switch to int??
};

namespace std {

    template<>
    struct hash<vertex> {
        std::size_t operator()(const vertex &v) const {
            return v.id;
        }
    };

}

bool relax(vertex from, vertex to, float weight) {
    bool changed = from.dist + weight < to.dist;

    if (changed) {
        to.dist = from.dist + weight;
    }

    return changed;
}

// graph as adjacency list
void dijkstra(std::map<vertex, std::vector<edge>> &graph,
              unsigned long source) {

    norsi::min_pqueue<vertex> Q(graph.size());
    Q.push({source, 0});

    while (!Q.empty()) {
        vertex u = Q.min_element();
        Q.pop();

        std::cout << graph[u].size() << std::endl;

        for (auto &edge : graph[u]) {
            vertex v = edge.target;    //FIXME: does this work? i.e. correct dist?

            float alt = u.dist + edge.weight;
            if (alt < v.dist) {
                v.dist = alt;
                //v.prev = u;
                if (!Q.contains(v)) {
                    Q.push(v);
                }
            }
        }
    }
}

int main(int argc, char **argv) {
    // TODO: check usage

    std::string path(argv[1]);
    std::map<vertex, std::vector<edge>> graph;
    std::ifstream r(path);
    std::string line;

    unsigned long id, edge_to;
    float weight;

    while (std::getline(r, line)) {
        id = std::stoul(line.substr(0, line.find(" ") - 1));
        edge_to = std::stoul(line.substr(line.find(" ") + 1, line.rfind(" ") - 1));
        weight = std::stof(line.substr(line.rfind(" ") + 1));

        graph[{id, HUGE_VALF}].push_back({{edge_to, HUGE_VALF}, weight});
    }

    for (const auto &p : graph) {
        if (p.second.size() > 5) {
            std::cout << p.first.id << std::endl;
            break;
        }
    }

    // second parameter is the source vertex
    dijkstra(graph, 45060697);

    return 0;
}
