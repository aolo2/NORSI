//
// Created by aolo2 on 7/10/17.
//

#include <vector>
#include <iostream>
#include <fstream>
#include <queue>
#include <limits>
#include <unordered_map>
#include <bits/unordered_set.h>

typedef std::pair<unsigned long, float> vertex;

void dijkstra(std::unordered_map<unsigned long, std::vector<vertex>> &graph, unsigned long source) {
    struct vertex_comp {
        bool operator()(const vertex &a, const vertex &b) { return a.second > b.second; }
    };

    std::unordered_map<unsigned long, float> dist;
    for (const auto &v : graph) { dist[v.first] = std::numeric_limits<float>::infinity(); }

    std::priority_queue<vertex, std::vector<vertex>, vertex_comp> Q;
    // dist[source] = 0.0f;

    Q.push({source, 0});

    std::unordered_set<unsigned long> q_set;

    q_set.insert(source);

    while (!Q.empty()) {
        auto u = Q.top();
        Q.pop();
        q_set.erase(u.first);

        if (u.second < dist[u.first]) {
            dist[u.first] = u.second;
        }

        for (const auto &v : graph[u.first]) {
            float alt = dist[u.first] + v.second;
            if (alt < dist[v.first]) {
                Q.push({v.first, alt});
                q_set.insert(v.first);
                dist[v.first] = alt;
            }
        }

    }

    for (const auto &entry : dist) {
        std::cout << std::left << std::fixed << entry.first << ":" << entry.second << std::endl;
    }

}

int main(int argc, char **argv) {
    std::string path(argv[1]);
    std::ifstream r(path);
    std::string line;

    std::unordered_map<unsigned long, std::vector<vertex>> graph;

    unsigned long id, edge_to;
    float weight;

    while (std::getline(r, line)) {
        id = std::stoul(line.substr(0, line.find(" ")));
        edge_to = std::stoul(line.substr(line.find(" ") + 1, line.rfind(" ")));
        weight = std::stof(line.substr(line.rfind(" ") + 1));

        graph[id].push_back({edge_to, weight});
    }

    dijkstra(graph, 1);

    return 0;
}
