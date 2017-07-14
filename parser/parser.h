//
// Created by Valifirt on 10/07/17.
//

#ifndef NORSI_GEO_2017_PARSER_OSM_H
#define NORSI_GEO_2017_PARSER_OSM_H

#include <string>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>

struct Node {
    float lat, lon;
    std::string id;
};

typedef std::pair<unsigned long, float> vertex;
struct vertex_comp {
    bool operator()(const vertex &a, const vertex &b) { return a.second > b.second; }
};

//parse osm and do grahp file. return p: p.first.second = map{id_in_map -> id_for_graph}, p.first.first = map {id_graph -> Node} p.second = map {id_graph -> ways}
std::pair<std::pair<std::map<unsigned int, Node>, std::map<std::string, unsigned int>>, std::unordered_map<unsigned int, std::vector<vertex>>> parser_osm(
        std::ifstream &in,
        std::string work_dir,
        std::string s);

//parse graph.  return p: p.first.second = map{id_in_map -> id_for_graph}, p.first.first = map {id_graph -> Node} p.second = map {id_graph -> ways}
std::pair<std::pair<std::map<unsigned int, Node>, std::map<std::string, unsigned int>>, std::unordered_map<unsigned int, std::vector<vertex>>> parser_graph(
        std::ifstream &in);

//output to osc file
void output_to_osc(std::pair<float,std::vector<unsigned int>> res, std::map<unsigned int,Node> map_nodes, std::string work_dir);

#endif //NORSI_GEO_2017_PARSER_OSM_H
