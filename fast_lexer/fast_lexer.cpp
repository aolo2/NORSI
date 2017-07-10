//
// Created by aolo2 on 7/9/17.
//

#include <iostream>
#include <unordered_set>
#include "fast_lexer.h"

std::pair<std::vector<Node>, std::vector<Way>> get_nodes_and_ways(std::ifstream &in) {
    std::string line;
    line.reserve(256);

    std::pair<std::vector<Node>, std::vector<Way>> ret;
    std::unordered_set<unsigned long> nodes;

    while (std::getline(in, line)) {
        if (line.find("<node") != std::string::npos) {
            Node node;
            node.id = std::stoul(line.substr(line.find("id=") + 4, line.find("\" ") - 1));
            node.lat = std::stof(line.substr(line.find("lat=") + 5, line.find(" lon") - 1));
            node.lon = std::stof(line.substr(line.find("lon=") + 5, line.rfind("\"") - 1));

            ret.first.push_back(node);
            nodes.insert(node.id);
        } else if (line.find("<way") != std::string::npos) {
            Way way;
            way.id = std::stoul(line.substr(line.find("id=") + 4, line.find("\" ") - 1));

            while (line.find("</way") == std::string::npos) {
                std::getline(in, line);
                if (line.find("<nd") != std::string::npos) {
                    unsigned long id_to_add = std::stoul(line.substr(line.find("ref=") + 5, line.rfind("\"") - 1));
                    if (nodes.find(id_to_add) != nodes.end()) {
                        way.nodes.push_back(id_to_add);
                    }
                }
            }

            if (way.nodes.size() > 1) {
                ret.second.push_back(way);
            }
        }
    }

    return ret;
};
