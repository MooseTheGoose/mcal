#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <glib.h>

typedef struct ParseTreeNode
{
    GSList node;
    struct ParseTreeNode *children;
}
ParseTreeNode;

ParseTreeNode *newPTreeNode(unsigned int variable);
void insertPTreeNode(ParseTreeNode *prev, ParseTreeNode *ins);
void unlinkPTreeNode(ParseTreeNode *parent, ParseTreeNode *rm);

#endif

