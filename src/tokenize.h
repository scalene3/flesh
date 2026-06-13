#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef enum { UNKNOWN, IDENTIFIER, END } Tag;

typedef struct Token {
        Tag tag;
        char *value;
        size_t valsize;
} Token;

typedef struct TokenList {
        Token *tokens;
        size_t size;
} TokenList;

/**
 * @brief Get the enum tag string.
 *
 * @param tag
 * @return char*
 */
char *getTagString(Tag tag);

/**
 * @brief Print tokens in list.
 *
 * @param list
 */
void printTokens(TokenList list);

/**
 * @brief Skip initial whitespace (newline and space). Modifies line.
 *
 * @param line
 * @param linesize
 */
void skipSpace(char **line, size_t linesize);

/**
 * @brief Get the next token, reading as alphanumeric string. Reads only
 * alphanumeric characters.
 *
 * @param line
 * @return Token
 */
Token getTokenAlphanum(char **line);

/**
 * @brief Get the next token from line
 *
 * @param line
 * @return Token
 */
Token getToken(char **line);

/**
 * @brief Tokenize the next line.
 *
 * @param line
 * @return TokenList
 */
TokenList tokenizeLine(char *line);