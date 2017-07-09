//
// Created by aolo2 on 7/9/17.
//

#ifndef NORSI_TOKENS_H
#define NORSI_TOKENS_H


/*
 * Грамматика (наверняка неполная)
 *
 * Statement      := Node | Way | Relation
 *
 * Node           := SingleLineNode | MultiLineNode
 * SingleLineNode := OPEN_BRACKET KW::NODE ParamsList Id GeoInfo CLOSE_BRACKET_INLINE
 * Id             := KW:ID EQUALS Quote NUMERIC_VALUE Quote
 * GeoInfo        := KW:LON | KW:LAT EQUALS Quote NUMERIC_VALUE Quote
 * Quote          := KW:QUOTE | KW:SINGLE_QUOTE
 * ParamsList     := KW EQUALS Quote Value Quote ParamsList | None
 * Value          := NUMERIC_VALUE | TEXT_VALUE
 * MultiLineNode  := OPEN_BRACKET KW:NODE ParamsList GeoInfo CLOSE_BRACKET
 *                := TagList
 *                := OPEN_BRACKET_INLINE KW:NODE CLOSE_BRACKET
 * TagList        := OPEN_BRACKET KW:TAG ParamsList CLOSE_BRACKET_INLINE
 *
 *
 * Way            := OPEN_BRACKET Id ParamsList CLOSE_BRACKET
 *                := NodeList
 *                := TagList
 *                := OPEN_BRACKET_INLINE KW:WAY CLOSE_BRACKET
 * NodeList       := OPEN_BRACKET KW:ND Ref CLOSE_BRACKET_INLINE NodeListEntry
 * NodeListEntry  := OPEN_BRACKET KW:ND Ref CLOSE_BRACKET_INLINE NodeListEntry | None
 * Ref            := KW:REF EQUALS Quote NUMERIC_VALUE Quote
 *
 * Relation       NOT PARSED FOR NOW
 *
 * */

enum KEYWORD_TYPE {
    VERSION,
    TIMESTAMP,
    UID,
    ID,
    USER,
    TAG,
    TYPE,
    CHANGESET,
    LAT,
    LON,
    REF,
    ROLE,
    ND,                     // вершина как элемент пути
    NODE,
    WAY,
    RELATION,
    MEMBER,
    V,                      // какое-то текстовое значение
    K,                      // тип?
};

enum TOKEN_TYPE {
    EQUALS,                 // =
    OPEN_BRACKET,           // <
    OPEN_BRACKET_INLINE,    // </
    CLOSE_BRACKET,          // >
    CLOSE_BRACKET_INLINE,   // />
    QUOTE,                  // "
    SINGLE_QUOTE,           // '
    KEYWORD_TOKEN,
    NUMERIC_VALUE,
    TEXT_VALUE,
    UNKNOWN,
};

struct Token {
    TOKEN_TYPE type;
    double numeric_value;
    std::string text_value;
    KEYWORD_TYPE keyword_type;
};

#endif //NORSI_TOKENS_H
