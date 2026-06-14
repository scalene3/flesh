#include "builtins.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

static int exportkv(char *arg) {
        char *eq = strchr(arg, '=');
        if (eq == arg) {
                return -1;
        }
        char *key = calloc((eq - arg), 1);
        char *value = calloc(strlen(eq), 1);
        if (!key || !value) {
                return -1;
        }

        key = memcpy(key, arg, eq - arg);
        value = memcpy(value, eq + 1, strlen(eq));
        if (!key || !value) {
                return -1;
        }
        setenv(key, value, 1);
        return 0;
}

int cd(char **args, size_t arg_count) {
        char *buf = malloc(512);
        char *cwd = getcwd(buf, 512);
        if (arg_count > 1) {
                if (!strcmp(args[1], "-")) {
                        char *oldpwd = getenv("OLDPWD");
                        if (chdir(oldpwd) == -1) {
                                perror("cd");
                                return errno;
                        }
                } else {
                        if (chdir(args[1]) == -1) {
                                perror("cd");
                                return errno;
                        } else {
                                setenv("OLDPWD", cwd, 1);
                        }
                }
        }
        return 0;
}

int export(char **args, size_t arg_count) {
        if (arg_count > 1) {
                for (size_t i = 1; i < arg_count; ++i) {
                        if (exportkv(args[i])) {
                                return 1;
                        } else {
                                continue;
                        }
                }
                return 0;
        }
        return 1;
}
