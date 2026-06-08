

#include "tokenize.h"

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