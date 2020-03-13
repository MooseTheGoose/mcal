#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <glib.h>

char **tokenize(char *expression);
void tokenizeNum(GString *tokens, char **number);
int isOperatorToken(int op);
void destroyTokens(GString *tokens);

#endif

