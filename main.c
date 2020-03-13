#include <stdio.h>
#include <assert.h>
#include "parse.h"
#include "tokenizer.h"
#include "decoder.h"

void printTokens(char **tokens)
{
    char *currToken;
    while((currToken = *tokens++)) 
    { printf("%s\n", currToken); }
}

int main(int argc, char **argv)
{
    GNode *parseTree;

    assert(argc == 2);
    char **tokens = tokenize(argv[1]);
    printTokens(tokens);
    parseTree = deriveStart(tokens);
    if(parseTree)
    {
        printf("Success!\n");
        outputParseTree(parseTree, stdout, 0);
        printf("%lf\n", decodeStart(parseTree));
        destroyParseTree(parseTree);
    }
    return 0;
}


