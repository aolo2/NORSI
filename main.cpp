#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <set>
#include <cmath>

const float R = 6371.0f;

struct node {
    long ID;
    float lon, lat;
};

struct way {
    long ID;
    node start, finish;
    float len = 0;

    bool operator<(const way rhs) const {
        return len < rhs.len;
    }
};

float deg2rad(float deg) {
    return deg * (M_PI / 180.0f);
}

float long_dist(node from, node to) {


    float dLat = deg2rad(to.lat - from.lat);  // deg2rad below
    float dLon = deg2rad(to.lon - from.lon);

    float a =
            std::sin(dLat / 2) * std::sin(dLat / 2) +
            std::cos(deg2rad(from.lat)) * std::cos(deg2rad(to.lat)) *
            std::sin(dLon / 2) * std::sin(dLon / 2);

    float c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    float d = R * c; // Distance in km

    return d;
}

float short_dist(node from, node to) {
    float dlat, dlon;
    dlat = to.lat - from.lat;
    dlon = to.lon - from.lon;

    return std::sqrt(dlat * dlat + dlon * dlon);
}

int main() {
    std::map<long, node> map_nodes;
    std::set<way> set_ways;

    // TODO: read path
    std::string line, path = "TY-graph.osm";
//    std::string line, path = "region.graph.osm";
    std::ifstream f(path);

    while (std::getline(f, line)) {
        std::size_t a;
        if ((a = line.find("<node")) != std::string::npos) {
            node tmp_node;
            a += 10;
            tmp_node.ID = std::stol(line.substr(a, line.find("\" ") - a));
            a = line.find("lat=") + 5;
            tmp_node.lat = std::stof(line.substr(a, line.find("\" lon") - a));
            a = line.find("lon=") + 5;
            tmp_node.lon = std::stof(line.substr(a, line.rfind("\"") - a));

            map_nodes[tmp_node.ID] = tmp_node;
        } else if ((a = line.find("<way")) != std::string::npos) {

            long last_node;
            bool first_iter = true;

            while (true) {
                std::getline(f, line);
                if (line.find("</way") != std::string::npos) { break; }
                if (line.find("<nd") != std::string::npos) {
                    a = line.find("\"") + 1;
                    if (first_iter){
                        first_iter = false;
                        last_node = std::stol(line.substr(a, line.rfind("\"") - a));
                    } else {
                        way current_way;

                        current_way.start = map_nodes[last_node];
                        last_node = std::stol(line.substr(a, line.rfind("\"") - a));
                        current_way.finish = map_nodes[last_node];
                        current_way.len = long_dist(current_way.start, current_way.finish);
                        set_ways.insert(current_way);
                    }
                }
            }
        }
    }



    std::cout << "graph {" << std::endl;
    for (auto n : map_nodes) {
        std::cout << "\t" << n.first << " [longitude=" << n.second.lon << ", latitude=" << n.second.lat << "]" << std::endl;
    }


    for (auto w : set_ways) {
        std::cout << "\t" << w.start.ID << " -- " << w.finish.ID << std::endl;
    }
    std::cout << "}" << std::endl;


    return 0;
}