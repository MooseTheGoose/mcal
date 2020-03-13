#include "parse.h"
#include "variables.h"
#include "sutils.h"
#include "error.h"
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int parseStart(GNode *parent, char *expr)
{
    GNode *startTree = g_node_new(GUINT_TO_POINTER(START));
    int status;
    
    printf("parseStart: %s\n",  expr);
    
    status = parseExp(startTree, expr) || parseMul(startTree, expr)
    || parseAdd(startTree, expr);
    
    g_node_insert(parent, 0, startTree);
    cleanOnFail(startTree, status);

    return status;
}

int parseExop(GNode *parent, char *exop)
{
    int status = 0;
    char *exopTrim = trim(exop);
    GNode *exopTree = g_node_new(GUINT_TO_POINTER(EXOP));
    
    printf("parseExop: %s\n", exopTrim);
    
    if(*exopTrim == '(')
    {
        char *exopTrimEnd = strrchr(exopTrim, ')');
        char *expr;
        int size;
                
        dieWhen(!exopTrimEnd, "Error: Mismatched left parantheses in expression");
        
        expr = substring(exopTrim+1, exopTrimEnd);
        
        GNode *left = g_node_new(GUINT_TO_POINTER('('));
        GNode *right = g_node_new(GUINT_TO_POINTER(')'));
        
        g_node_insert(exopTree, 0, right);
        status = parseStart(exopTree, expr);
        g_node_insert(exopTree, 0, left);
        
        g_free(expr);
        
    }
    else
    {
        status = parseNum(exopTree, exopTrim);
    }
    
    g_free(exopTrim);
    g_node_insert(parent, 0, exopTree);
    cleanOnFail(exopTree, status);
    
    return status;
}

int parseExp(GNode *parent, char *exp)
{
    GNode *expTree = g_node_new(GUINT_TO_POINTER(EXP));
    char *karat = strchr(exp, '^');
    int status = 0;
    
    printf("parseExp: %s\n", exp);
    
    if(karat)
    {
        char *lh = exp, *rh = karat + 1;
        char *lefthand = substring(lh, karat);
        char *righthand = substring(rh, rh + strlen(rh));
        
        GNode *kar = g_node_new(GUINT_TO_POINTER('^'));
        
        if(parseExop(expTree, righthand))
        {
            status = parseExp(expTree, lefthand);
        }
        else
        {
            status = parseExp(expTree, righthand) && parseExop(expTree, lefthand);
        }
        
        g_node_insert(expTree, 1, kar);
        
        g_free(lefthand);
        g_free(righthand);
        
    }
    else
    {
        status = parseExop(expTree, exp);
    }
    
    g_node_insert(parent, 0, expTree);
    cleanOnFail(expTree, status);
    
    return status;
}

int parseMulop(GNode *parent, char *mulop)
{
    GNode *mulopTree = g_node_new(GUINT_TO_POINTER(MULOP));
    
    int status = parseExp(mulopTree, mulop) || parseExop(mulopTree, mulop);
    
    g_node_insert(parent, 0, mulopTree);
    
    cleanOnFail(mulopTree, status);
    
    return 1;
}

int parseMul(GNode *tree, char *mul)
{
    return 1;
}

int parseAdop(GNode *tree, char *adop)
{
    return 1;
}

int parseAdd(GNode *tree, char *add)
{
    return 1;
}

int parseNum(GNode *parent, char *numStart)
{
    char *numEnd = numStart;
    int strSize;
    int status = isdigit(*numStart);
    GNode *numberNode = g_node_new(GUINT_TO_POINTER(NUMBER));
    GNode *storeNumber = g_node_new(NULL);
    
    printf("parseNum: %s\n", numStart);
    
    while(isdigit(*numEnd)) numEnd++;
    
    if(*numEnd == '.')
    {
        do { numEnd++; } while(isdigit(*numEnd));
    }
    
    numStart = substring(numStart, numEnd);
    
    storeNumber->data = numStart;
    
    status = status && !*numEnd;
    
    g_node_insert(parent, 0, numberNode);
    g_node_insert(numberNode, 0, storeNumber);
    
    cleanOnFail(numberNode, status);
    
    return status;
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

gboolean freeNumber(GNode *node, gpointer dummy)
{
    if(GPOINTER_TO_INT(node->data) == NUMBER)
    {
        g_free(node->children->data);
    }
    
    return FALSE;
}

void destroyParseTree(GNode *root)
{
    g_node_traverse(root, G_IN_ORDER, G_TRAVERSE_ALL, -1, freeNumber, NULL);
    g_node_destroy(root);
}

void cleanOnFail(GNode *root, int status)
{
    if(!status)
    {
        g_node_unlink(root);
        destroyParseTree(root);
    }
}

void outputParseTree(GNode *root, FILE *xml, int indent)
{
    char indentation[indent+1];
    GNode *child;
    char *tagName;
    
    for(int i = 0; i < indent; i++) indentation[i] = ' ';
    indentation[indent] = '\0';
    
    child = root->children;
    
    switch(GPOINTER_TO_INT(root->data))
    {
        case START:
        {
            tagName = "Start";
            break;
        }
        case EXOP:
        {
            tagName = "Exop";
            break;
        }
        case EXP:
        {
            tagName = "Exp";
            break;
        }
        case NUMBER:
        {
            tagName = "Number";
            break;
        }
        case '(':
        {
            tagName = "(";
            break;
        }
        case ')':
        {
            tagName = ")";
            break;
        }
        case '^':
        {
            tagName = "^";
            break;
        }
        default:
        {
            fprintf(xml, "%s%s\n", indentation, root->data);
            return;
        }
    }
    
    fprintf(xml, "%s<%s>\n", indentation, tagName);
    while(child)
    {
        outputParseTree(child, xml, indent + 4);
        child = child->next;
    }
    fprintf(xml, "%s</%s>\n", indentation, tagName);
}
