#include "builtins.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

static int exportkv(char *arg) {
        int retval = 0;
        char *eq = strchr(arg, '=');
        char *key = calloc((eq - arg), 1);
        char *value = calloc(strlen(eq), 1);
        if (eq == arg) {
                retval = -1;
                goto exit;
        }

        if (!key || !value) {
                retval = -1;
                goto exit;
        }

        key = memcpy(key, arg, eq - arg);
        value = memcpy(value, eq + 1, strlen(eq));
        if (!key || !value) {
                retval = -1;
                goto exit;
        }
        setenv(key, value, 1);
exit:
        free(arg);
        free(key);
        free(value);
        return retval;
}

int cd(char **args, size_t arg_count) {
        int retval = 0;
        char *buf = malloc(512);
        char *cwd = getcwd(buf, 512);
        if (arg_count > 1) {
                if (!strcmp(args[1], "-")) {
                        char *oldpwd = getenv("OLDPWD");
                        if (chdir(oldpwd) == -1) {
                                perror("cd");

                                retval = errno;
                                goto exit;
                        }
                } else {
                        if (chdir(args[1]) == -1) {
                                perror("cd");
                                retval = errno;
                                goto exit;
                        } else {
                                setenv("OLDPWD", cwd, 1);
                                goto exit;
                        }
                }
        }
exit:
        free(buf);
        free(args);
        return retval;
}

int export(char **args, size_t arg_count) {
        int retval = 1;
        if (arg_count > 1) {
                for (size_t i = 1; i < arg_count; ++i) {
                        if (exportkv(args[i])) {
                                retval = 1;
                                goto exit;
                        } else {
                                continue;
                        }
                }
                retval = 0;
                goto exit;
        }
exit:
        free(args);
        return retval;
}
