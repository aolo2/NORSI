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
    unsigned long id;
};

struct Way {
    std::vector<unsigned long> nodes;
    unsigned long id;
};


std::pair<std::vector<Node>, std::vector<Way>> get_nodes_and_ways(std::ifstream &in);

#endif //NORSI_FAST_LEXER_H
