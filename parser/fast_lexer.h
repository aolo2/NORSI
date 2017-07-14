//
// Created by aolo2 on 7/9/17.
//

#ifndef NORSI_FAST_LEXER_H
#define NORSI_FAST_LEXER_H

#include <vector>
#include <string>
#include <fstream>

struct Node {
    float lat, lon;
    std::string id;
};

struct Way {
    std::vector<std::string> nodes;
    std::string id;
};


std::pair<std::vector<Node>, std::vector<Way>> get_nodes_and_ways(std::ifstream &in);

#endif //NORSI_FAST_LEXER_H
