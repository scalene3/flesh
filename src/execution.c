
#include "execution.h"
#include "builtins.h"
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
                } else {
                        free(args);
                        return 0;
                }
        }
        // null-terminate args
        args = realloc(args, (arg_count + 1) * sizeof(char *));
        args[arg_count] = nullptr;

        printf("arg_count: %zu\n", arg_count);

        if (!strcmp(args[0], "exit")) {
                int exitcode = 0;
                if (arg_count > 1) {
                        exitcode = atoi(args[1]);
                }
                free(args);
                exit(exitcode);
        } else if (!strcmp(args[0], "cd")) {
                return cd(args, arg_count);
        } else if (!strcmp(args[0], "export")) {
                return export(args, arg_count);
        }

        fflush(NULL);
        pid_t p = fork();
        if (p < 0) {
                perror("fork");
                free(args);
                exit(EXIT_FAILURE);
        } else if (!p) {

                printf("args: \n");
                for (size_t i = 0; i < arg_count; ++i) {
                        printf("  %s\n", args[i]);
                }
                execvp(args[0], args);

        } else {
                int status = 0;
                wait(&status);
                int exit_status = WEXITSTATUS(status);
                if (exit_status == 1) {
                        printf("flesh: command not found: %s\n", args[0]);
                }
                free(args);
                return exit_status;
        }
        free(args);
        exit(EXIT_FAILURE);
}
