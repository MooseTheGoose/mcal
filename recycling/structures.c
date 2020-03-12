#include "structures.h"
#include <glib.h>
#include <stdio.h>

ParseTreeNode *newPTreeNode(unsigned int variable)
{
    ParseTreeNode *newNode = calloc(1, sizeof(ParseTreeNode));
    
    if(newNode)
    {
        newNode->node.data = GUINT_TO_POINTER(variable);
    }
    else
    {
        fprintf(stderr, "Error in newParseTreeNode(): couldn't allocate memory for new node\n");
    }
    
    return newNode;
}

void insertPTreeNode(ParseTreeNode *prev, ParseTreeNode *ins)
{
    prev->node.next = &ins->node;
}

void unlinkPTreeNode(ParseTreeNode *parent, ParseTreeNode *rm)
{
    parent->children = (ParseTreeNode *)g_slist_remove_link(&parent->children->node, &rm->node);
}




