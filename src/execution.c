
#include "execution.h"
#include "builtins.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

int execArgs(char *command, char **args) {
        int exit_status;
        pid_t p = fork();
        if (p < 0) {
                exit_status = errno;
                goto cleanup;
        } else if (!p) {

                // printf("args: \n");
                // for (size_t i = 0; i < arg_count; ++i) {
                //      printf("  %s\n", args[i]);
                // }
                execvp(command, args);
                exit_status = errno;
                goto cleanup;
        } else {
                int status = 0;
                wait(&status);
                exit_status = WEXITSTATUS(status);
                if (exit_status == 1) {
                        printf("flesh: command not found: %s\n", command);
                }
                goto cleanup;
        }
cleanup:
        free(args);
        free(command);
        return exit_status;
}

size_t parseArgs(TokenList list, char ***args) {
        *args = malloc(sizeof(char *));
        size_t arg_count = 0;
        for (size_t i = 0; i < list.size; ++i) {
                // Currently ignore non-IDENTIFIER tokens
                // keep getting tokens as arguments if token.tag == IDENTIFIER
                if (list.tokens[i].tag == IDENTIFIER) {
                        *args =
                            realloc(*args, (arg_count + 1) * sizeof(char *));
                        (*args)[arg_count] = list.tokens[i].value;
                        ++arg_count;
                } else {
                        // no args or invalid token. either way, free and finish
                        // prompt
                        free(args);
                        return 0;
                }
        }
        return arg_count;
}

int execTokens(TokenList list) {
        char **args = nullptr;
        size_t arg_count;
        int exitcode = 0;
        arg_count = parseArgs(list, &args);

        if (!arg_count) {
                return 0;
        }

        if (!strcmp(args[0], "exit")) {

                if (arg_count > 1) {
                        exitcode = atoi(args[1]);
                }
                exit(exitcode);

        } else if (!strcmp(args[0], "cd")) {
                return cd(args, arg_count);
        } else if (!strcmp(args[0], "export")) {
                return export(args, arg_count);
        }

        fflush(NULL);
        return execArgs(args[0], args);
}
