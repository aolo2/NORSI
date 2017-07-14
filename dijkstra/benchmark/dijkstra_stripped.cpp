//
// Created by aolo2 on 7/14/17.
//

#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>
#include <queue>

typedef unsigned int id;
typedef std::pair<id, float> vertex;
typedef std::unordered_map<id, std::vector<vertex>> graph;
typedef std::pair<float, std::vector<id>> path;

struct vertex_comp {
    bool operator()(const vertex &a, const vertex &b) { return a.second > b.second; }
};

path dijkstra(graph &G, id from, id to) {
    std::priority_queue<vertex, std::vector<vertex>, vertex_comp> Q;
    std::unordered_map<id, float> dist;
    std::unordered_map<id, id> prev;

    for (const auto &v : G) {
        dist[v.first] = std::numeric_limits<float>::infinity();
    }

    Q.push({from, 0});
    while (!Q.empty()) {

        auto u = Q.top();
        Q.pop();

        if (u.second < dist[u.first]) {
            dist[u.first] = u.second;
        }

        for (const auto &v : G[u.first]) {
            float alt = dist[u.first] + v.second;
            if (alt < dist[v.first]) {
                Q.push({v.first, alt});
                dist[v.first] = alt;
                prev[v.first] = u.first;
            }
        }

        if (u.first == to) {
            path res;

            res.first = u.second;
            res.second.push_back(to);

            for (auto v = prev[to]; v != from; v = prev[v]) {
                res.second.push_back(v);
            }

            res.second.push_back(from);
            return res;
        }
    }

    std::cout << "unreachable" << std::endl;
    return {-1, {}};
}

void usage() {
    std::cerr << "Usage: GRAPH_FILE from to [to ...]" << std::endl;
    std::exit(0);
}

int main(int argc, char **argv) {
    if (argc < 4) { usage(); }

    std::string path(argv[1]);
    std::ifstream graph_file(path);

    if (!graph_file) { usage(); }

    graph G;
    float weight;
    id unique_id = 0;
    std::string line, from, to;
    std::unordered_map<std::string, id> string_to_id;
    std::unordered_map<id, std::string> id_to_string;

    while (std::getline(graph_file, line)) {
        std::size_t edge;
        edge = line.find(" ") + 1;

        from = line.substr(0, line.find(" "));
        to = line.substr(edge, line.rfind(" ") - edge);
        weight = std::stof(line.substr(line.rfind(" ") + 1));

        if (string_to_id.find(from) == string_to_id.end()) {
            string_to_id[from] = unique_id;
            id_to_string[unique_id] = from;
            ++unique_id;
        }

        if (string_to_id.find(to) == string_to_id.end()) {
            string_to_id[to] = unique_id;
            id_to_string[unique_id] = to;
            ++unique_id;
        }

        G[string_to_id[from]].push_back({string_to_id[to], weight});
        G[string_to_id[to]].push_back({string_to_id[from], weight});
    }

    std::ofstream of("LEN_int.sdot");

    for (auto &n : G) {
        for (auto &e : n.second) {
            of << n.first << " " << e.first << " " << e.second << std::endl;
        }
    }

   /* std::vector<id> stops;
    for (unsigned int i = 2; i < argc; i++) {
        stops.push_back(string_to_id[std::string(argv[i])]);
    }

    for (unsigned int i = 0; i < stops.size() - 1; i++) {
        std::cout << stops[i + 1] << "->" << stops[i] << ":" << std::endl;
        for (auto v : dijkstra(G, stops[i], stops[i + 1]).second) {
            std::cout << id_to_string[v] << std::endl;
        }
        std::cout << std::endl;
    }*/

    return 0;
}