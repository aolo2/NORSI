//
// Created by aolo2 on 7/9/17.
//

#ifndef NORSI_LEXER_H
#define NORSI_LEXER_H

#include <vector>
#include <string>
#include "tokens.h"

std::pair<Token, unsigned int> get_keyword(const std::string &src, unsigned int index);

std::vector<Token> get_tokens(const std::string &src);

#endif //NORSI_LEXER_H
