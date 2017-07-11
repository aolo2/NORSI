//
// Created by valya on 10/07/17.
//

#ifndef NORSI_GEO_2017_PARSER_OSM_H
#define NORSI_GEO_2017_PARSER_OSM_H

#include <string>
#include <fstream>
#include <map>
#include <vector>

struct Node {
    float lat, lon;
    unsigned long id;
};

//parse osm and do grahp file + return two maps
std::pair<std::map<unsigned long, Node>, std::map<unsigned long, std::vector<std::pair<unsigned long, float>>>> parser_osm(std::ifstream &in,std::string s);

//parse graph and return two maps
std::pair<std::map<unsigned long, Node>, std::map<unsigned long, std::vector<std::pair<unsigned long, float>>>> parser_graph(std::ifstream &in);

#endif //NORSI_GEO_2017_PARSER_OSM_H
