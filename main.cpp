#include <iostream>
#include <map>
#include <vector>
#include<iterator>
#include <fstream>
#include <set>
#include <cmath>
#include <algorithm>

#include "parser.h"
#include "dijkstra.h"

int main(int argc, char **argv) {

    if (argc < 4){
        std::cout << "Error arg" << std::endl;
        exit(1);
    }

    std::string path = argv[1];

    std::string name_file;
    name_file = path.substr(path.rfind("/") + 1, path.rfind(".") - path.rfind("/")-1);

    std::string work_dir = path.substr(0,path.rfind("/osm") + 1);

    std::ifstream f;

    std::pair<std::pair<std::map<unsigned int, Node>, std::map<std::string, unsigned int>>, std::unordered_map<unsigned int, std::vector<vertex>>>  p;

    f.open(work_dir + "graph/" + name_file + "_graph");

    if (f){
        p = parser_graph(f);
    } else {
        f.open(path);
        if (!f){
            std::cout << "Can't find *.osm file" << std::endl;
            exit(2);
        }
        p = parser_osm(f, work_dir, name_file);
    }

    std::vector<unsigned int> route;

    for(int i = 2; i < argc ; i++){
        std::string point = argv[i];
        if (p.first.second.find(point) == p.first.second.end()){
            std::cout << "No point " << point << " in map" << std::endl;
            exit(3);
        }
        route.push_back(p.first.second[point]);
    }

    std::pair<float, std::vector<unsigned int>> c;

    for(int i = 0; i < route.size()-1; i++){
        auto res = dijkstra(p.second, route[i], route[i+1]);
        c.first += res.first;
        std::copy(res.second.begin(), res.second.end(), back_inserter(c.second));
    }

    output_to_osc(c, p.first.first, work_dir);

    return 0;
}