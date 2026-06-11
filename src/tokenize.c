
#include "tokenize.h"
#include <ctype.h>

char *getTagString(Tag tag) {
    switch (tag) {
    case UNKNOWN:
        return "UNKNOWN";
    case IDENTIFIER:
        return "IDENTIFIER";
    case END:
        return "END";
    default:
        perror("Invalid tag");
        exit(EXIT_FAILURE);
    }
}

void printTokens(TokenList list) {
    for (size_t i = 0; i < list.size; ++i) {
        printf("#%zu: ", i);
        printf("tag: %s ", getTagString(list.tokens[i].tag));
        printf("value: %s\n", list.tokens[i].value);
    }
}

void skipSpace(char **line, size_t linesize) {
    size_t pos = 0;
    while (pos < linesize) {
        if ((*line)[pos] == ' ' || (*line)[pos] == '\n') {
            (*line)[pos] = '\0';
            ++pos;
        } else {
            break;
        }
    }
    *line += pos;
}

Token getTokenAlphanum(char **line) {
    size_t sliceto = 1;
    while ((*line)[sliceto] && isalnum((*line)[sliceto])) {
        ++sliceto;
    }
    Token token = {};
    token.tag = IDENTIFIER;
    token.value = *line;
    token.valsize = sliceto;
    *line += sliceto;
    return token;
}

Token getToken(char **line) {
    size_t pos = 0;
    char first = (*line)[pos];
    if (isalpha(first) || first == '_') {
        return getTokenAlphanum(line);
    } else if (first == '\\') {
        fprintf(stderr, "escape characters not yet implemented");
        exit(1);
    } else {
        *line += (pos + 1);
        Token token;
        if (!first) {

            token.tag = END;
        } else {
            token.tag = UNKNOWN;
        }
        token.value = &first;
        return token;
    }
}

TokenList tokenizeLine(char *line) {
    TokenList list = {};
    if (!line) {
        list.tokens = nullptr;
        list.size = 0;
        return list;
    }
    Token *tokens = (Token *)malloc(sizeof(Token));
    size_t allocatedsize = 0;

    do {
        skipSpace(&line, strlen(line));
        Token token = getToken(&line);
        if (token.tag == END) {
            break;
        }
        tokens = realloc(tokens, (allocatedsize + 1) * sizeof(Token));
        if (!tokens) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        tokens[allocatedsize] = token;
        ++allocatedsize;
    } while (*line);
    list.size = allocatedsize;
    list.tokens = tokens;
    return list;
}