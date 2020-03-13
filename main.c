#include <stdio.h>
#include <assert.h>
#include "parse.h"
#include "tokenizer.h"
#include "decoder.h"
#include "error.h"

int main(int argc, char **argv)
{
    GNode *parseTree;

    if(argc != 2)
    {
        puts("Usage: mcal 'math-expression'");
        exit(-1);
    }

    char **tokens = tokenize(argv[1]);
    parseTree = deriveStart(tokens);
    dieWhen(!parseTree, "Error: Invalid expression");


    if(parseTree)
    {
        printf("%lf\n", decodeStart(parseTree));
        destroyParseTree(parseTree);
    }

    return 0;
}


