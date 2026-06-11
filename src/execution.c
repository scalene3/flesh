
#include "execution.h"
#include <sys/wait.h>

int execTokens(TokenList list) {
    char **args = malloc(sizeof(char *));

    size_t arg_count = 0;
    for (size_t i = 0; i < list.size; ++i) {
        // Currently ignore non-IDENTIFIER tokens
        // keep getting tokens as arguments if token.tag == IDENTIFIER
        if (list.tokens[i].tag == IDENTIFIER) {
            args = realloc(args, (arg_count + 1) * sizeof(char *));
            args[arg_count] = list.tokens[i].value;
            ++arg_count;
        }
    }

    fflush(NULL);
    pid_t p = fork();
    if (p < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (!p) {
        printf("executing command: %s\n", args[0]);
        execvp(args[0], args);

    } else {
        int status = 0;
        wait(&status);
        int exit_status = WEXITSTATUS(status);
        return exit_status;
    }

    exit(EXIT_FAILURE);
}