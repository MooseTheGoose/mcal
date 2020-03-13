#include <stdio.h>
#include <assert.h>
#include "parse.h"
#include "tokenizer.h"

void printToken(gpointer tok, gpointer dummy);

int main(int argc, char **argv)
{
    assert(argc == 2);
    GQueue *tokens = tokenize(argv[1]);
    g_queue_foreach(tokens, printToken, NULL);
    destroyTokens(tokens);
    return 0;
}

void printToken(gpointer tok, gpointer dummy)
{
    printf("%s\n", tok);
}
