//
// Created by Valifirt on 10/07/17.
//

#include "parser.h"

#include <math.h>
#include <cmath>
#include <iostream>
#include <unordered_set>
#include <sys/stat.h>
#include <ctime>

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

std::pair<std::pair<std::map<unsigned int, Node>, std::map<std::string, unsigned int>>, std::unordered_map<unsigned int, std::vector<vertex>>> parser_osm(
        std::ifstream &in,
        std::string work_dir,
        std::string s) {

    std::map<unsigned int, Node> map_nodes;
    std::map<std::string, unsigned int> map_inf;
    std::unordered_map<unsigned int, std::vector<vertex>> map_ways;

    std::string line;

    unsigned int number = 0;

    std::ofstream out, out_print;

    while (std::getline(in, line)) {
        if (line.find("<node") != std::string::npos) {
            Node node;

            std::size_t id, lat, lon;
            id = line.find("id=") + 4;
            lat = line.find("lat=") + 5;
            lon = line.find("lon=") + 5;

            node.id = line.substr(id, line.find("\" ") - id);
            node.lat = std::stof(line.substr(lat, lon - 7 - lat));
            node.lon = std::stof(line.substr(lon, line.rfind("\"") - lon));

            map_inf.insert({node.id,number});
            map_nodes.insert({number,node});
            std::vector<vertex> v;
            map_ways.insert({number, v});

            number++;

        } else if (line.find("<way") != std::string::npos) {

            bool k = true;
            unsigned int first_node;
            std::string l;

            while (true) {
                std::getline(in, line);
                if (line.find("</way") != std::string::npos) { break; }
                if (line.find("<nd") != std::string::npos) {
                    if (k) {
                        std::size_t a = line.find("ref=") + 5;
                        l = line.substr(a, line.rfind("\"") - a );
                        if (map_inf.find(l) != map_inf.end()){
                            k = false;
                            first_node = map_inf[l];
                        }
                    } else {
                        unsigned int second_node;
                        std::size_t a = line.find("ref=") + 5;
                        l = line.substr(a, line.rfind("\"") - a );
                        second_node = map_inf[l];
                        if (map_inf.find(l) != map_inf.end()){
                            float len = long_dist(map_nodes[first_node], map_nodes[second_node]);
                            map_ways[first_node].push_back({second_node, len});
                            map_ways[second_node].push_back({first_node, len});
                            first_node = second_node;
                        }
                    }
                }
            }
        }
    }

    // version for parse and future work, version for print
    /*
     * out creates *_graph. Format:
     *             name of map
     *             n of nodes
     *             number_node id_in_map lat lon
     *             ...
     *             number_first_node number_second_node len
     *             ...
     *
     * out_for_print creates *.dot
     * */

    out.open(work_dir + "graph/" + s + "_graph");
    out_print.open(work_dir + "graph/" + s + "_graph_for_print.dot");

    out << s << "\n" << number << std::endl;
    out_print << "graph {" << std::endl;

    for (auto v : map_nodes){
        out << v.first << " " << v.second.id << " "  << v.second.lat << " " << v.second.lon << "\n";
        out_print << "\t" << number << " [latitude =" << v.second.lat << ", longitude=" << v.second.lon << "]" << std::endl;
    }

    for (auto v : map_ways){
        for (auto w : v.second){
            out << v.first << " " << w.first << " " << w.second << "\n";
            out_print << "\t" << v.first << " -- " << w.first << std::endl;
        }
    }

    out_print << "}" << std::endl;

    out.close();
    out_print.close();

    return {{map_nodes, map_inf}, map_ways};
}

std::pair<std::pair<std::map<unsigned int, Node>, std::map<std::string, unsigned int>>, std::unordered_map<unsigned int, std::vector<vertex>>> parser_graph(
        std::ifstream &in) {

    std::map<unsigned int, Node> map_nodes;
    std::map<std::string, unsigned int> map_inf;
    std::unordered_map<unsigned int, std::vector<vertex>> map_ways;

    std::string line;

    std::getline(in, line);                     // name of file
    unsigned int n_nodes;
    in >> n_nodes;

    for( ;n_nodes > 0; n_nodes--){
        Node node;
        unsigned int number;
        in >> number >> node.id >> node.lat >> node.lon;

        map_inf.insert({node.id, number});
        map_nodes.insert({number, node});
        std::vector<vertex> v;
        map_ways.insert({number,v});
    }

    unsigned int first, second;
    float len;
    while( in >> first >> second >> len ){
        map_ways[first].push_back({second,len});
        map_ways[second].push_back({first,len});
    }

    return {{map_nodes, map_inf},map_ways};
};

void output_to_osc(std::pair<float, std::vector<unsigned int>> way,std::map<unsigned int,Node> map_nodes, std::string work_dir) {

    std::string dir_out = work_dir + "out_way";

    mkdir(dir_out.c_str(), 0777);                        //create a dir {0 - create, -1 - exits}

    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime (&rawtime);

    std::string name_file;
    name_file += std::to_string(timeinfo->tm_year + 1900);
    name_file += "_" + std::to_string(timeinfo->tm_mon);
    name_file += "_" + std::to_string(timeinfo->tm_mday) ;
    name_file += "_" + std::to_string(timeinfo->tm_hour);
    name_file += ":" + std::to_string(timeinfo->tm_min);
    name_file += ":" + std::to_string(timeinfo->tm_sec);

    std::ofstream w(dir_out + "/" + name_file + ".osc");
    w << "<osmChange version=\"0.6\" generator=\"Norsi\">\n"
            "\t<create>\n\t<way id=\"-1\">\n";


    for (const auto &node : way.second) {
        w << "\t\t<nd ref=\"" << map_nodes[node].id << "\"/>" << std::endl;
    }

    w << "\t</way>\n\t<relation id=\"-2\">\n"
            "\t\t<member type=\"way\" ref=\"-1\" role=\"route\"/>\n"
            "\t\t<tag k=\"type\" v=\"route\"/>\n"
            "\t\t<tag k=\"route\" v=\"road\"/>\n"
            "\t\t<tag k=\"colour\" v=\"red\"/>\n"
            "\t\t<tag k=\"distance\" v=\"" << way.first << "\"/>\n"
              "\t</relation>\n\t</create>\n</osmChange>";
    w.close();
}