//
// Created by aolo2 on 7/10/17.
//

#include <unordered_map>
#include <fstream>
#include <cmath>
#include <map>
#include <queue>

#include "min_pqueue/min_pqueue.hpp"

bool relax(vertex from, vertex to, float weight) {
    bool changed = from.dist + weight < to.dist;

    if (changed) {
        to.dist = from.dist + weight;
    }

    return changed;
}

// graph as adjacency list
void dijkstra(std::map<unsigned long, std::vector<edge>> &graph,
              unsigned long source) {

    // Dijkstra-NoDec
    std::priority_queue Q;
}

int main(int argc, char **argv) {
/*
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

*/




    return 0;
}
