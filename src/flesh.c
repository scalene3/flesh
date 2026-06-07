
#include <ctype.h>
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
        printf("allocated_size: %zu\n", allocatedsize);
        list.tokens = tokens;
        return list;
}

void promptLoop(FILE *tty) {
        for (;;) {
                char *line = nullptr;
                size_t size = 0;

                ssize_t nread = getline(&line, &size, tty);
                if (nread == -1) {
                        perror("getline");
                        exit(EXIT_FAILURE);
                }
                TokenList tokens = tokenizeLine(line);
                printTokens(tokens);
        }
}

int main() {
        if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) {
                perror("isatty");
                exit(EXIT_FAILURE);
        }
        char *tname = ttyname(STDIN_FILENO);
        FILE *tty = fopen(tname, "r+");

        promptLoop(tty);
        exit(EXIT_SUCCESS);
}