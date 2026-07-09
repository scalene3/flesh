#include "tokenize.h"

/**
 * @brief execute provided args. Does not return if sucessful
 *
 * @param command
 * @param args
 * @return int
 */
int execArgs(char *command, char **args);

/**
 * @brief read tokens into an array of strings.
 *
 * @param list
 * @param args pointer to empty array of strings.
 * @return size_t
 */
size_t parseArgs(TokenList list, char ***args);

/**
 * @brief execute Tokens appropriately. Returns exit code
 *
 * @param tokens
 * @return int
 */
int execTokens(TokenList tokens);
