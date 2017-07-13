//
// Created by aolo2 on 7/10/17.
//

#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>
#include <limits>
#include <unordered_map>
#include <unordered_set>

typedef std::pair<std::string, float> vertex;
typedef std::pair<float, std::vector<std::string>> path;
typedef std::unordered_map<std::string, std::vector<vertex>> graph;

struct vertex_comp {
    bool operator()(const vertex &a, const vertex &b) { return a.second > b.second; }
};

path dijkstra(graph &graph, const std::string &source, const std::string &target) {
    std::priority_queue<vertex, std::vector<vertex>, vertex_comp> Q;
    std::unordered_map<std::string, float> dist;
    std::unordered_map<std::string, std::string> prev;

    for (const auto &v : graph) {
        dist[v.first] = std::numeric_limits<float>::infinity();
    }

    Q.push({source, 0});
    while (!Q.empty()) {

        auto u = Q.top();
        Q.pop();

        if (u.second < dist[u.first]) {
            dist[u.first] = u.second;
        }

        for (const auto &v : graph[u.first]) {
            float alt = dist[u.first] + v.second;
            if (alt < dist[v.first]) {
                Q.push({v.first, alt});
                dist[v.first] = alt;
                prev[v.first] = u.first;
            }
        }

        if (u.first == target) {
            path res;

            res.first = u.second;
            res.second.push_back(target);

            for (auto v = prev[target]; v != source; v = prev[v]) {
                res.second.push_back(v);
            }

            res.second.push_back(source);
            return res;
        }
    }

    std::cout << "unreachable" << std::endl;
    return {-1, {}};
}

void output_to_osc(const path &p) {
    std::ofstream w("routes/route.osc");
    w << "<osmChange version=\"0.6\" generator=\"Norsi\">\n"
            "\t<create>\n\t<way id=\"-1\">\n";

    for (const auto &node : p.second) {
        w << "\t\t<nd ref=\"" << node << "\"/>" << std::endl;
    }

    w << "\t</way>\n\t<relation id=\"-2\">\n"
            "\t\t<member type=\"way\" ref=\"-1\" role=\"route\"/>\n"
            "\t\t<tag k=\"type\" v=\"route\"/>\n"
            "\t\t<tag k=\"route\" v=\"road\"/>\n"
            "\t\t<tag k=\"colour\" v=\"red\"/>\n"
            "\t\t<tag k=\"distance\" v=\"" << p.first << "\"/>\n"
              "\t</relation>\n\t</create>\n</osmChange>";
}

int main(int argc, char **argv) {
    std::string path(argv[1]);
    std::ifstream r(path);
    std::string line;

    std::unordered_map<std::string, std::vector<vertex>> graph;

    std::string id, edge_to;
    float weight;

    while (std::getline(r, line)) {
        std::size_t edge;
        edge = line.find(" ") + 1;

        id = line.substr(0, line.find(" "));
        edge_to = line.substr(edge, line.rfind(" ") - edge);
        weight = std::stof(line.substr(line.rfind(" ") + 1));

        graph[id].push_back({edge_to, weight});
        graph[edge_to].push_back({id, weight});
    }

    std::string source, target;
    std::cin >> source >> target;

    output_to_osc(dijkstra(graph, source, target));

    return 0;
}
