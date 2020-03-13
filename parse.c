#include "parse.h"
#include "strvutils.h"
#include "error.h"
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

GNode *deriveStart(char **tokens)
{
    GNode *parseTree = NULL;
    char *currToken = *tokens;

    if(currToken)
    {
        GNode *children[3];

        if((children[0] = deriveNum(tokens))
            || (children[0] = deriveUnaryMinus(tokens))
            || (children[0] = deriveAdd(tokens))
            || (children[0] = deriveMult(tokens))
            || (children[0] = deriveExp(tokens)))
        {
            parseTree = g_node_new(g_strdup("START"));
            g_node_prepend(parseTree, children[0]);
        }
        else if(!strcmp(currToken, "("))
        {
            char **rightMostPara = strvrstr(++tokens, ")");
            char **newTokens;
            
            if(rightMostPara)
            {
                newTokens = strndupv(tokens, rightMostPara - tokens);
                
                if(newTokens && !*(rightMostPara + 1)
                   && (children[1] = deriveStart(newTokens)))
                {
                    parseTree = g_node_new(g_strdup("START"));
                    children[0] = g_node_new(g_strdup("("));
                    children[2] = g_node_new(g_strdup(")"));
                    g_node_prepend(parseTree, children[2]);
                    g_node_prepend(parseTree, children[1]);
                    g_node_prepend(parseTree, children[0]);
                }
                g_free(newTokens);
            }
        }

    }     

    return parseTree;
}

GNode *deriveNum(char **tokens)
{
    GNode *parseTree = NULL;
    char *currToken = *tokens;
    
    if(currToken && isdigit(*currToken) && !*(tokens + 1))
    {
        GNode *child = g_node_new(g_new(double, 1));
        *(double *)child->data = strtod(currToken, NULL);
        parseTree = g_node_new(g_strdup("NUMBER"));
        g_node_prepend(parseTree, child);
    }
    
    return parseTree;
}

GNode *deriveMultOp(char **tokens)
{
    GNode *parseTree = NULL;
    GNode *child;
    
    if((child = deriveExp(tokens)) || (child = deriveExpOp(tokens)))
    {
        parseTree = g_node_new(g_strdup("MULTOP"));
        g_node_prepend(parseTree, child);
    }
    
    return parseTree;
}

GNode *deriveMult(char **tokens)
{
    GNode *parseTree = NULL;
    char **newTokens;
    
    char **operator;
    
    if((operator = strvrstr(tokens, "*"))
        || (operator = strvrstr(tokens, "/")))
    {
        GNode *children[3];
        newTokens = strndupv(tokens, operator - tokens);
        
        if((children[2] = deriveMultOp(operator + 1)))
        {
            if((children[0] = deriveMult(newTokens))
               || (children[0] = deriveMultOp(newTokens)))
            {
                parseTree = g_node_new(g_strdup("MULT"));
                children[1] = g_node_new(g_strdup(*operator));
                g_node_prepend(parseTree, children[2]);
                g_node_prepend(parseTree, children[1]);
                g_node_prepend(parseTree, children[0]);
            }
            else
            {
                destroyParseTree(children[2]);
            }
        }
        
        g_free(newTokens);
    }
    
    return parseTree;
}

GNode *deriveAddOp(char **tokens)
{
    GNode *parseTree = NULL;
    GNode *child;
    
    if((child = deriveMultOp(tokens)) || (child = deriveMult(tokens)))
    {
        parseTree = g_node_new(g_strdup("ADDOP"));
        g_node_prepend(parseTree, child);
    }
    return parseTree;
}

GNode *deriveAdd(char **tokens)
{
    GNode *parseTree = NULL;
    char **newTokens;
    char **operator;
    char *first = *tokens;
    GNode *child;
    
    if((operator = strvrstr(tokens, "+"))
        || (operator = strvrstr(tokens, "-")))
    {
        GNode *children[3];
        newTokens = strndupv(tokens, operator - tokens);
        
        if((children[2] = deriveAddOp(operator + 1)))
        {
            if((children[0] = deriveAdd(newTokens))
                || (children[0] = deriveAddOp(newTokens)))
            {
                parseTree = g_node_new(g_strdup("ADD"));
                children[1] = g_node_new(g_strdup(*operator));
                g_node_prepend(parseTree, children[2]);
                g_node_prepend(parseTree, children[1]);
                g_node_prepend(parseTree, children[0]);
            }
            else
            {
                destroyParseTree(children[2]);
            }
        }
        
        g_free(newTokens);
    }
    
    return parseTree;
}

GNode *deriveUnaryMinusOp(char **tokens)
{
    GNode *parseTree = NULL;
    char *first = *tokens;
    GNode *child;
    char **temp = tokens;
    
    
    if(first && !strcmp(first, "("))
    {
        GNode *children[3];
        
        char **rightMostPara = strvrstr(++tokens, ")");
        char **newTokens;
        
        if(rightMostPara)
        {
            newTokens = strndupv(tokens, rightMostPara - tokens);
            
            if(newTokens && !*(rightMostPara + 1)
               && (children[1] = deriveStart(newTokens)))
            {
                parseTree = g_node_new(g_strdup("UNARYMINUSOP"));
                children[0] = g_node_new(g_strdup("("));
                children[2] = g_node_new(g_strdup(")"));
                g_node_prepend(parseTree, children[2]);
                g_node_prepend(parseTree, children[1]);
                g_node_prepend(parseTree, children[0]);
            }
            g_free(newTokens);
        }
    }
    else if((child = deriveNum(tokens)))
    {
        parseTree = g_node_new(g_strdup("UNARYMINUSOP"));
        g_node_prepend(parseTree, child);
    }
    
    return parseTree;
}

GNode *deriveUnaryMinus(char **tokens)
{
    GNode *parseTree = NULL;
    GNode *child;
    char *first = *tokens;

    if(first && !strcmp(first, "-") && (child = deriveUnaryMinusOp(tokens + 1)))
    {
        parseTree = g_node_new(g_strdup("UNARYMINUS"));
        g_node_prepend(parseTree, child);
        g_node_prepend(parseTree, g_node_new(g_strdup("-"))); 
    }

    return parseTree;
}

GNode *deriveExpOp(char **tokens)
{
    GNode *parseTree = NULL;
    GNode *child;
    
    if((child = deriveUnaryMinusOp(tokens)) || (child = deriveUnaryMinus(tokens)))
    {
        parseTree = g_node_new(g_strdup("EXPOP"));
        g_node_prepend(parseTree, child);
    }
    return parseTree;
}

GNode *deriveExp(char **tokens)
{
    GNode *parseTree = NULL;
    GNode *children[3];
    char **operator = strvrstr(tokens, "^");
    char **temp;
    char **newToken;
    
    if(operator && (children[2] = deriveExpOp(operator + 1)))
    {
        char **newTokens = strndupv(tokens, operator - tokens);
        
        if((children[0] = deriveExp(newTokens))
            || (children[0] = deriveExpOp(newTokens)))
        {
            parseTree = g_node_new(g_strdup("EXP"));
            children[1] = g_node_new(g_strdup("^"));
            g_node_prepend(parseTree, children[2]);
            g_node_prepend(parseTree, children[1]);
            g_node_prepend(parseTree, children[0]);
        }
        else
        {
            destroyParseTree(children[2]);
        }
        g_free(newTokens);
    }
    
    return parseTree;
}

gboolean freeNodeData(GNode *node, gpointer dummy)
{
    g_free(node->data);
    return FALSE;
}

void destroyParseTree(GNode *root)
{
    g_node_traverse(root, G_IN_ORDER, G_TRAVERSE_ALL, -1, freeNodeData, NULL);
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
    
    for(int i = 0; i < indent; i++) indentation[i] = ' ';
    indentation[indent] = '\0';
    
    child = root->children;
    
    fprintf(xml, "%s<%s>\n", indentation, root->data);
    
    if(!strcmp(root->data, "NUMBER"))
    {
        fprintf(xml, "%s%lf\n", indentation, *(double *)child->data);
    }
    else
    {
        while(child)
        {
            outputParseTree(child, xml, indent + 4);
            child = child->next;
        }
    }

    fprintf(xml, "%s</%s>\n", indentation, root->data);
}

