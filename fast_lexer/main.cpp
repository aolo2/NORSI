//
// Created by aolo2 on 7/9/17.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>

#include "fast_lexer.h"

const float earthRadius = 6371.0f;
const float PI = 3.1415926f;

float deg2rad(float deg) {
    return (deg * PI / 180.0f);
}

float distanceEarth(std::pair<float, float> coords1, std::pair<float, float> coords2) {
    float lat1r, lon1r, lat2r, lon2r, u, v;

    lat1r = deg2rad(coords1.first);
    lon1r = deg2rad(coords1.second);
    lat2r = deg2rad(coords2.first);
    lon2r = deg2rad(coords2.second);

    u = std::sin((lat2r - lat1r) / 2);
    v = std::sin((lon2r - lon1r) / 2);

    return 1000.0f * 2.0f * earthRadius *
            std::asin(std::sqrt(u * u + std::cos(lat1r) * std::cos(lat2r) * v * v));
}

void output_gephi(const std::pair<std::vector<Node>, std::vector<Way>>
                  &nodes_and_ways, std::ofstream &w) {
    w << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" <<
    "<gexf version=\"1.1\">\n" <<
    "<graph defaultedgetype=\"undirected\" idtype=\"string\" type=\"static\">\n" <<
    "<attributes class=\"node\" mode=\"static\">\n" <<
    "<attribute id=\"latitude\" title=\"latitude\" type=\"double\"/>\n" <<
    "<attribute id=\"longitude\" title=\"longitude\" type=\"double\"/>\n" <<
    "</attributes>" << std::endl;

    w << "<nodes count=\"" << nodes_and_ways.first.size() << "\">" << std::endl;

    for (const auto &node : nodes_and_ways.first) {
        w << "<node id=\"" << node.id << "\">\n" <<
        "<attvalues>\n" <<
        "<attvalue for=\"latutide\" value=\"" << node.lat << "\"/>\n" <<
        "<attvalue for=\"longitude\" value=\"" << node.lon << "\"/>\n" <<
        "</attvalues>\n" <<
        "</node>" << std::endl;
    }

    w << "</nodes>" << std::endl;
    w << "<edges count=\"" << nodes_and_ways.second.size() << "\">" << std::endl;

    for (const auto &way : nodes_and_ways.second) {
        for (unsigned int i = 0; i < way.nodes.size() - 1; i++) {
            w << "<edge id=\"" << way.id << "\" source=\"" << way.nodes[i] <<
            "\" target=\"" << way.nodes[i + 1] << "\"/>" << std::endl;
        }
    }

    w << "</edges>\n</graph>\n</gexf>" << std::endl;
}

void output_sdot(const std::pair<std::vector<Node>, std::vector<Way>>
                 &nodes_and_ways, std::ofstream &w) {
    std::map<unsigned long, std::pair<float, float>> id_to_coords;

    for (const auto &node : nodes_and_ways.first) {
        id_to_coords[node.id] = {node.lat, node.lon};
    }

    for (const auto &way : nodes_and_ways.second) {
        for (unsigned int i = 0; i < way.nodes.size() - 1; i++) {
            w << way.nodes[i] << " " << way.nodes[i + 1] << " " <<
            distanceEarth(id_to_coords[way.nodes[i]], id_to_coords[way.nodes[i + 1]]) << std::endl;
        }
    }
}

void usage_error() {
    std::cerr << "Usage: ./osmparse PATH-TO-INPUT.osm -d(-g)" << std::endl;
}

int main(int argc, char **argv) {

    if (argc != 3) {
        usage_error();
        return -1;
    }

    std::string path(argv[1]);
    std::ifstream r(path);
    std::string key(argv[2]);

    if (key != "-g" && key != "-d") { usage_error(); }

    std::pair<std::vector<Node>, std::vector<Way>>
            nodes_and_ways = get_nodes_and_ways(r);

    if (key == "-g") {
        std::ofstream out("out/roads.gexf");
        output_gephi(nodes_and_ways, out);
    } else {
        std::ofstream out("out/roads.sdot");
        output_sdot(nodes_and_ways, out);
    }

    return 0;
}