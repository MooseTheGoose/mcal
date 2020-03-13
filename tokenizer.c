#include "tokenizer.h"
#include "sutils.h"
#include "error.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

static const char *OPERATORS = "()+-/*^";

void tokenizeNum(GQueue *tokens, char **number)
{
    int len = 1;
    char *num = *number;

    while(isdigit(num[len])) len++;
    if(num[len] == '.')
    {
        len++;
        dieWhen(!isdigit(num[len]), "Error: expected number in tokenizeNum at beginning of %s\n", num);
        while(isdigit(num[len])) len++;
    }
    g_queue_push_tail(tokens, g_strndup(num, len));

    *number = num + len;
}

GQueue *tokenize(char *expression)
{
    int prefix;
    GQueue *tokens = g_queue_new();

    while(*expression)
    {
        while(isspace(prefix = *expression)) { expression++; }

        if(isOperatorToken(prefix))
        {
            g_queue_push_tail(tokens, g_strndup(expression++, 1));
        }
        else if(isdigit(prefix))
        {
            tokenizeNum(tokens, &expression);
        }
        else
        {
            dieWhen(prefix, "Error: Unknown token at prefix of %s\n", expression);
        }
    }

    return tokens;
}

void destroyTokens(GQueue *tokens)
{
    gpointer *curr = g_queue_pop_head(tokens);
    while(curr)
    {
        g_free(curr);
        curr = g_queue_pop_head(tokens);
    }
    g_queue_free(tokens);
}

int isOperatorToken(int op)
{
    return strchr(OPERATORS, op) != NULL && op;
}

