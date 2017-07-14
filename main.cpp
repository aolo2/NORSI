#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <set>
#include <cmath>

#include "parser.h"

int main() {

    std::string path = "/home/valya/ClionProjects/NORSI_geo_2017/OSM_files/RU-TY.osm";

//    std::string path;
//    std::cin >> path;

    std::string name_file;
    name_file = path.substr(path.rfind("/") + 1, path.rfind(".") - path.rfind("/") - 1);
    std::ifstream f;
    std::pair<std::map<unsigned long, Node>, std::map<unsigned long, std::vector<std::pair<unsigned long, float>>>> p;

    f.open(path.substr(0, path.rfind("OSM") - 1) + "/graph/" + name_file + "_graph");
    if (f) {
        std::cout << "file graph is ready" << std::endl;
        p = parser_graph(f);
    } else {
        f.open(path);
        p = parser_osm(f, name_file);
    }

    return 0;
}