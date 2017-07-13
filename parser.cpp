//
// Created by valya on 10/07/17.
//

#include "parser.h"

#include <math.h>
#include <cmath>
#include <iostream>
#include <unordered_set>

const float R = 6371.0f;

float deg2rad(float deg) {
    return deg * (M_PI / 180.0f);
}

float long_dist(Node from, Node to) {


    float dLat = deg2rad(to.lat - from.lat);  // deg2rad below
    float dLon = deg2rad(to.lon - from.lon);

    float a =
            std::sin(dLat / 2) * std::sin(dLat / 2) +
            std::cos(deg2rad(from.lat)) * std::cos(deg2rad(to.lat)) *
            std::sin(dLon / 2) * std::sin(dLon / 2);

    float c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    float d = R * c; // Distance in km

    return d*1000;
}

float short_dist(Node from, Node to) {
    float dlat, dlon;
    dlat = to.lat - from.lat;
    dlon = to.lon - from.lon;

    return std::sqrt(dlat * dlat + dlon * dlon);
}

std::pair<std::map<unsigned long, Node>, std::unordered_map<unsigned long, std::vector<vertex>>> parser_osm(std::ifstream &in, std::string s) {

    std::pair<std::map<unsigned long, Node>, std::unordered_map<unsigned long, std::vector<vertex>>> p;
    std::unordered_set<unsigned long> set_id;

    std::string line;
    bool flag = true;
    // version for parse and fufure work, version for print
    std::ofstream out, out_print;

    out.open("/home/valya/ClionProjects/NORSI_geo_2017/graph/" + s + "_graph");
    out_print.open("/home/valya/ClionProjects/NORSI_geo_2017/graph/" + s + "_graph_for_print.dot");

    out << s << "\n" << "Nodes" << "\n";
    out_print << "graph {" << std::endl;

    while (std::getline(in, line)) {
        if (line.find("<node") != std::string::npos) {
            Node node;
            node.id = std::stoul(line.substr(line.find("id=") + 4, line.find("\" ") - 1));
            node.lat = std::stof(line.substr(line.find("lat=") + 5, line.find(" lon") - 1));
            node.lon = std::stof(line.substr(line.find("lon=") + 5, line.rfind("\"") - 1));

            p.first.insert(std::pair<unsigned  long, Node>(node.id,node));
            set_id.insert(node.id);
            std::vector<vertex> v;
            p.second.insert(std::pair<unsigned long, std::vector<vertex>>(node.id, v));
            out << node.id << " " << node.lat << " " << node.lon << "\n";
            out_print << "\t" << node.id << " [latitude =" << node.lat << ", longitude=" << node.lon << "]" << std::endl;
        } else if (line.find("<way") != std::string::npos) {

            if (flag) {
                out << "Ways" << "\n";
                flag = false;
            }

            bool k = true;
            unsigned long first_node;

            while (true) {
                std::getline(in, line);
                if (line.find("</way") != std::string::npos) { break; }
                if (line.find("<nd") != std::string::npos) {
                    if (k) {
                        first_node = std::stoul(line.substr(line.find("ref=") + 5, line.rfind("\"") - 1));
                        if (set_id.find(first_node) != set_id.end()){
                            k = false;
                        }
                    } else {
                        unsigned long second_node;
                        second_node = std::stoul(line.substr(line.find("ref=") + 5, line.rfind("\"") - 1));
                        if (set_id.find(second_node) != set_id.end()){
                            float len = long_dist(p.first[first_node],p.first[second_node]);
                            p.second[first_node].push_back(std::pair<unsigned long, float>(second_node, len));
                            p.second[second_node].push_back(std::pair<unsigned long, float>(first_node, len));
                            out << first_node << " " << second_node << " " << len << "\n";
                            out_print << "\t" << first_node << " -- " << second_node << std::endl;
                            first_node = second_node;
                        }
                    }
                }
            }
        }
    }

    out_print << "}" << std::endl;

    out.close();
    out_print.close();

    return p;
}

std::pair<std::map<unsigned long, Node>, std::unordered_map<unsigned long, std::vector<vertex>>> parser_graph(std::ifstream &in) {

    std::pair<std::map<unsigned long, Node>, std::unordered_map<unsigned long, std::vector<vertex>>> p;
    std::string line;

    std::getline(in, line);                     // name of file
    std::getline(in, line);                     // Nodes

    while (std::getline(in,line) && line.find("Ways") == std::string::npos){
        Node n;
        n.id = std::stoul(line.substr(0, line.find(" ")), nullptr, 10);
        n.lat = std::stof(line.substr(line.find(" ") + 1, line.rfind(" ") - line.find(" ") - 1));
        n.lon = std::stof(line.substr(line.rfind(" ")));

        p.first.insert(std::pair<unsigned long, Node>(n.id, n));
        std::vector<vertex> v;
        p.second.insert(std::pair<unsigned long,std::vector<vertex>>(n.id, v));
    }

    while (std::getline(in,line)){
        unsigned long id = std::stoul(line.substr(0, line.find(" ")), nullptr, 10);
        unsigned long id_next;
        float len;
        id_next = std::stoul(line.substr(line.find(" ")+1, line.find(" ") - line.find(" ")-1), nullptr, 10);
        len = std::stof(line.substr(line.rfind(" ")));
        p.second[id].push_back(std::pair<unsigned long, float>(id_next, len));
        p.second[id_next].push_back(std::pair<unsigned long, float>(id, len));
    }

    return p;
};

//void output_to_osc(std::pair<float, std::vector<unsigned long>> way, std::string name) {
//    std::ofstream w("/home/valya/ClionProjects/NORSI_geo_2017/OUT_WAY/"+ name + ".osc");
//    w << "<osmChange version=\"0.6\" generator=\"Norsi\">\n"
//            "\t<create>\n\t<way id=\"-1\">\n";
//
//
//    for (const auto &node : way.second) {
//        w << "\t\t<nd ref=\"" << node << "\"/>" << std::endl;
//    }
//
//    w << "\t</way>\n\t<relation id=\"-2\">\n"
//            "\t\t<member type=\"way\" ref=\"-1\" role=\"route\"/>\n"
//            "\t\t<tag k=\"type\" v=\"route\"/>\n"
//            "\t\t<tag k=\"route\" v=\"road\"/>\n"
//            "\t\t<tag k=\"colour\" v=\"red\"/>\n"
//            "\t\t<tag k=\"distance\" v=\"" << way.first << "\"/>\n"
//              "\t</relation>\n\t</create>\n</osmChange>";
//    w.close();
//}