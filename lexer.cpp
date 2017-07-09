//
// Created by aolo2 on 7/9/17.
//

#include <cstring>
#include <iostream>
#include "lexer.h"

std::vector<Token> get_tokens(const std::string &src) {
    std::vector<Token> tokens;
    std::string quoted_value;

    bool text_value = true;
    bool inside_quotes = false;
    std::pair<Token, unsigned int> keyword;
    char c;


    for (unsigned int i = 0; i < src.size(); i++) {
        c = src[i];

        if (std::isspace(c)) {
            continue;
        }

        if (inside_quotes) {
            if (c == '\"') {

                if (text_value) {
                    tokens.push_back({TOKEN_TYPE::TEXT_VALUE, 0, quoted_value});
                } else {
                    if (keyword.first.keyword_type == KEYWORD_TYPE::VERSION ||
                        keyword.first.keyword_type == KEYWORD_TYPE::UID ||
                        keyword.first.keyword_type == KEYWORD_TYPE::ID ||
                        keyword.first.keyword_type == KEYWORD_TYPE::CHANGESET ||
                        keyword.first.keyword_type == KEYWORD_TYPE::REF) {
                        tokens.push_back({TOKEN_TYPE::NUMERIC_VALUE, std::stol(quoted_value), ""});
                    } else {
                        tokens.push_back({TOKEN_TYPE::NUMERIC_VALUE, std::stof(quoted_value), ""});
                    }
                }

                tokens.push_back({TOKEN_TYPE::QUOTE});
                inside_quotes = false;
            } else {
                quoted_value += c;
            }

            continue;
        }

        switch (c) {
            case '\"':
                tokens.push_back({TOKEN_TYPE::QUOTE});
                inside_quotes = true;
                quoted_value.clear();
                break;
            case '\'':
                tokens.push_back({TOKEN_TYPE::SINGLE_QUOTE});
                inside_quotes = true;
                quoted_value.clear();
                break;
            case '=':
                tokens.push_back({TOKEN_TYPE::EQUALS});
                break;
            case '<':
                if (src[i + 1] == '/') {
                    tokens.push_back({TOKEN_TYPE::OPEN_BRACKET_INLINE});
                    ++i;
                } else {
                    tokens.push_back({TOKEN_TYPE::OPEN_BRACKET});
                }
                break;
            case '/':
                if (src[i + 1] == '>') {
                    tokens.push_back({TOKEN_TYPE::CLOSE_BRACKET_INLINE});
                    ++i;
                } else {
                    tokens.push_back({TOKEN_TYPE::UNKNOWN});
                }
                break;
            case '>':
                tokens.push_back({TOKEN_TYPE::CLOSE_BRACKET});
                break;
            default:
                keyword = get_keyword(src, i);
                if (keyword.first.type != TOKEN_TYPE::UNKNOWN) {
                    tokens.push_back({TOKEN_TYPE::KEYWORD_TOKEN, 0, "", keyword.first.keyword_type});

                    text_value = !(keyword.first.keyword_type == KEYWORD_TYPE::VERSION ||
                                   keyword.first.keyword_type == KEYWORD_TYPE::UID ||
                                   keyword.first.keyword_type == KEYWORD_TYPE::ID ||
                                   keyword.first.keyword_type == KEYWORD_TYPE::CHANGESET ||
                                   keyword.first.keyword_type == KEYWORD_TYPE::LAT ||
                                   keyword.first.keyword_type == KEYWORD_TYPE::LON ||
                                   keyword.first.keyword_type == KEYWORD_TYPE::REF);

                    i += keyword.second;
                } else {
                    std::cerr << "Unkown keyword at " << i << std::endl;
                }
                break;
        }

    }

    return tokens;
}

std::pair<Token, unsigned int> get_keyword(const std::string &src, unsigned int index) {

    if (src.size() < index + 1) return {{TOKEN_TYPE::UNKNOWN}, 0};

    unsigned int keyword_len = 1;
    std::string keyword_text = "";
    KEYWORD_TYPE keyword_type;

    char c = src[index];

    switch (src[index]) {
        case 'o':
            keyword_type = KEYWORD_TYPE::OSM;
            break;
        case 'v':
            keyword_type = (src[index + 1] == '=') ? KEYWORD_TYPE::V : KEYWORD_TYPE::VERSION;
            break;
        case 't':
            if (src[index + 1] == 'a') {
                keyword_type = KEYWORD_TYPE::TAG;
            } else {
                keyword_type = (src[index + 1] == 'y') ? KEYWORD_TYPE::TYPE : KEYWORD_TYPE::TIMESTAMP;
            }
            break;
        case 'u':
            keyword_type = (src[index + 1] == 'i') ? KEYWORD_TYPE::UID : KEYWORD_TYPE::USER;
            break;
        case 'c':
            keyword_type = KEYWORD_TYPE::CHANGESET;
            break;
        case 'l':
            keyword_type = (src[index + 1] == 'a') ? KEYWORD_TYPE::LAT : KEYWORD_TYPE::LON;
            break;
        case 'r':
            if (src[index + 1] == 'e') {
                keyword_type = (src[index + 2] == 'f') ? KEYWORD_TYPE::REF : KEYWORD_TYPE::RELATION;
            } else {
                keyword_type = KEYWORD_TYPE::ROLE;
            }
            break;
        case 'n':
            keyword_type = (src[index + 1] == 'd') ? KEYWORD_TYPE::ND : KEYWORD_TYPE::NODE;
            break;
        case 'w':
            keyword_type = KEYWORD_TYPE::WAY;
            break;
        case 'm':
            keyword_type = KEYWORD_TYPE::MEMBER;
            break;
        case 'k':
            keyword_type = KEYWORD_TYPE::K;
            break;
        case 'i':
            keyword_type = KEYWORD_TYPE::ID;
            break;
        default:
            return {{TOKEN_TYPE::UNKNOWN}, 0};
    }

    switch (keyword_type) {
        case OSM:
            keyword_text = "osm";
            keyword_len = 3;
            break;
        case VERSION:
            keyword_text = "version";
            keyword_len = 7;
            break;
        case TIMESTAMP:
            keyword_text = "timestamp";
            keyword_len = 9;
            break;
        case TYPE:
            keyword_text = "type";
            keyword_len = 4;
            break;
        case UID:
            keyword_text = "uid";
            keyword_len = 3;
            break;
        case ID:
            keyword_text = "id";
            keyword_len = 2;
            break;
        case USER:
            keyword_text = "user";
            keyword_len = 4;
            break;
        case TAG:
            keyword_text = "tag";
            keyword_len = 3;
            break;
        case CHANGESET:
            keyword_text = "changeset";
            keyword_len = 9;
            break;
        case LAT:
            keyword_text = "lat";
            keyword_len = 3;
            break;
        case LON:
            keyword_text = "lon";
            keyword_len = 3;
            break;
        case REF:
            keyword_text = "ref";
            keyword_len = 3;
            break;
        case ROLE:
            keyword_text = "role";
            keyword_len = 4;
            break;
        case ND:
            keyword_text = "nd";
            keyword_len = 2;
            break;
        case NODE:
            keyword_text = "node";
            keyword_len = 4;
            break;
        case WAY:
            keyword_text = "way";
            keyword_len = 3;
            break;
        case RELATION:
            keyword_text = "relation";
            keyword_len = 8;
            break;
        case MEMBER:
            keyword_text = "member";
            keyword_len = 6;
            break;
        case V:
            keyword_text = "v";
            keyword_len = 1;
            break;
        case K:
            keyword_text = "k";
            keyword_len = 1;
            break;
    }


    if (std::strncmp(&src[index], keyword_text.c_str(), keyword_len) == 0) {
        return {
                {TOKEN_TYPE::KEYWORD_TOKEN, 0, "", keyword_type}, keyword_len - 1};
    } else {
        return {{TOKEN_TYPE::UNKNOWN}, 0};
    }
}
