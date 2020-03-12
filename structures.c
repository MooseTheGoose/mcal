//
//  structures.c
//  
//
//  Created by Youssef Moosa on 3/11/20.
//

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




