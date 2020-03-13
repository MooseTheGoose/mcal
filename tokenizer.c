#include "tokenizer.h"
#include "error.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

static const char *OPERATORS = "()+-/*^";

void tokenizeNum(GString *tokens, char **number)
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
    g_string_append_len(tokens, num, len);

    *number = num + len;
}

char **tokenize(char *expression)
{
    int prefix;
    GString *tokens = g_string_new("");
    char **finalTokens;
    char *tmp;
    char *ptr = expression;

    while(*ptr)
    {
        while(isspace(prefix = *ptr)) ptr++;

        if(isOperatorToken(prefix))
        {
            g_string_append_len(tokens, ptr++, 1);
        }
        else if(isdigit(prefix))
        {
            tokenizeNum(tokens, &ptr);
        }
        else
        {
            dieWhen(prefix, "Error: Unknown token in %s at char %d\n", 
                    expression, ptr-expression+1);
        }
        g_string_append_c(tokens, ' ');
    }

    g_string_truncate(tokens, MAX(0, tokens->len-1));
    tmp = g_string_free(tokens, FALSE);
    g_strchomp(tmp);
    finalTokens = g_strsplit(tmp, " ", -1);
    g_free(tmp);

    return finalTokens;
}

void destroyTokens(GString *tokens)
{
    g_string_free(tokens, TRUE);
}

int isOperatorToken(int op)
{
    return strchr(OPERATORS, op) != NULL && op;
}

