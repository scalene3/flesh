

#include "execution.h"

void promptLoop(FILE *tty) {
    while (1) {
        char *line = nullptr;
        size_t size = 0;
        putchar('>');
        putchar(' ');
        ssize_t nread = getline(&line, &size, tty);
        if (nread == -1) {
            perror("getline");
            exit(EXIT_FAILURE);
        }
        TokenList tokens = tokenizeLine(line);

        int exit = execTokens(tokens);
        printf("exit code: %d\n", exit);
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