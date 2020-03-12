#include "parse.h"
#include "variables.h"
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void dieWhen(int condition, char *lastWords)
{
    if(condition) { fprintf(stderr, "%s\n", lastWords); }
    exit(-1);
}

char *substring(const char *first, const char *erase)
{
    int size = erase - first;
    char *sub = g_malloc(size + 1);
    memcpy(sub, first, size);
    sub[size] = '\0';
    return sub;
}

int parseStart(GNode *tree, char *expr)
{
    return parseExp(tree, expr) || parseMul(tree, expr)
              || parseAdd(tree, expr);
}

int parseExop(GNode *tree, char *exop)
{
    int status = 0;
    
    while(isspace(*exop)) exop++;
    
    if(*exop == '(')
    {
        char *exopEnd = strrchr(exop, ')');
        char *expr;
        int size;
        
        dieWhen(!exopEnd, "Error: Mismatched left parantheses in expression");
        
        expr = substring(exop+1, exopEnd);
        
        GNode *left = g_node_new(GUINT_TO_POINTER('('));
        GNode *newTree = g_node_new(GUINT_TO_POINTER(START));
        GNode *right = g_node_new(GUINT_TO_POINTER(')'));
        
        g_node_insert(tree, -1, left);
        g_node_insert(tree, -1, newTree);
        g_node_insert(tree, -1, right);

        status = parseStart(newTree, expr);
        
        g_free(expr);
    }
    else
    {
        GNode *newTree = g_node_new(NULL);
        status = parseNum(tree, exop);
    }
    
    return status;
}

int parseExp(GNode *tree, char *exp)
{
    char *karat = strchr('^');
    int status = 0;
    
    if(karat)
    {
        char *lh = exp, *rh = karat + 1;
        char *lefthand = substring(lh, karat);
        char *righthand = substring(rh, rh + strlen(rh));
        
        GNode *left = g_node_new(NULL), *right = g_node_new(NULL);
        
        g_free(lefthand);
        g_free(righthand);
    }
    else
    {
        GNode *newTree = g_node_new(GUINT_TO_POINTER(EXOP));
        status = parseExop(tree, exp);
    }
    
    return 0;
}

int parseMulop(GNode *tree, char *mulop)
{
    return 0;
}

int parseMul(GNode *tree, char *mul)
{
    return 0;
}

int parseAdop(GNode *tree, char *adop)
{
    return 0;
}

int parseAdd(GNode *tree, char *add)
{
    return 0;
}

int parseNum(GNode *tree, char *numStart)
{
    char *numEnd = numStart;
    char *number;
    int strSize;
    
    assert(isdigit(*numStart));
    
    while(isdigit(*numEnd)) numEnd++;
    if(*numEnd == '.')
    {
        while(isdigit(*numEnd)) numEnd++;
    }
    strSize = numEnd - numBegin + 1;
    number = malloc(strSize);
    if(number)
    {
        memcpy(number, numStart, strSize-1);
        number[strSize-1] = '\0';
    }
    else
    {
        fprintf(stderr,"Error in parseNum(): Could not allocate memory for num");
    }
    
    tree->data = number;
    
    return 1;
}

double decodeNumber(char *num)
{
    double integ = 0.0;
    double frac = 0.0;
    double div = 1.0;
    int currDigit = *num;
    
    assert(isdigit(currDigit));

    do
    {
        integ *= 10.0;
        integ += (double)(currDigit - '0');
        currDigit = *++num;
    }
    while(isdigit(currDigit));

    if(currDigit == '.')
    {
        currDigit = *++num;
        assert(isdigit(currDigit));
        do
        {
            frac *= 10.0;
            div *= 10.0;
            frac += (double)(currDigit - '0');
            currDigit = *++num;
        }
        while(isdigit(currDigit));
        assert(!currDigit);
        frac /= div;
    }
   
    return integ + frac;
}

