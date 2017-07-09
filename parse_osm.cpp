//
// Created by aolo2 on 7/9/17.
//

#include <iostream>
#include <fstream>
#include <vector>

#include "tokens.h"
#include "lexer.h"

const float R = 6371.0f;
const float PI = 3.1415926f;

int main(int argc, char **argv) {
    std::string line, path(argv[1]);
    std::vector<Token> tokens;

    std::ifstream f(path);

    while (std::getline(f, line)) {
        tokens = get_tokens(line);
    }

    return 0;
}