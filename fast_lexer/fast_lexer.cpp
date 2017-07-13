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

    while (std::getline(in, line)) {
        if (line.find("<node") != std::string::npos) {
            Node node;

            std::size_t id, lat, lon;
            id = line.find("id=") + 4;
            lat = line.find("lat=") + 5;
            lon = line.find("lon=") + 5;

            node.id = line.substr(id, line.find("\" ") - id);
            node.lat = std::stof(line.substr(lat, lon - 1));
            node.lon = std::stof(line.substr(lon, line.rfind("\"") - lon));

            ret.first.push_back(node);
        } else if (line.find("<way") != std::string::npos) {
            Way way;
            way.id = std::stoul(line.substr(line.find("id=") + 4, line.find("\" ")));

            while (line.find("</way") == std::string::npos) {
                std::getline(in, line);
                if (line.find("<nd") != std::string::npos) {
                    std::size_t ref = line.find("ref=") + 5;
                    std::string id_to_add = line.substr(ref, line.rfind("\"") - ref);
                    way.nodes.push_back(id_to_add);
                }
            }

            if (way.nodes.size() > 1) {
                ret.second.push_back(way);
            }
        }
    }

    return ret;
};
