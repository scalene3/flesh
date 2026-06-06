
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

typedef struct Token {
        enum { UNKNOWN, STRING } tag;
        char *value;
} Token;

Token *tokenizeLine(char *line) {
        size_t linesize = strlen(line);
        Token *tokens = (Token *)malloc(sizeof(Token) << 2);

        while (line && *line) {
                char c = *line;
                // TODO: split on whitespace and word boundaries
        }

        return tokens;
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
                Token *tokens = tokenizeLine(line);
                fwrite(line, nread, 1, stdout);
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