#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <glib.h>

GQueue *tokenize(char *expression);
void tokenizeNum(GQueue *tokens, char **number);
int isOperatorToken(int op);
void destroyTokens(GQueue *tokens);

#endif

