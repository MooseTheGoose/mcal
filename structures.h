#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct ParseTreeNode
{
    GSList node;
    struct ParseTreeNode *children;
}
ParseTreeNode;

ParseTreeNode *newPTreeNode(unsigned int variable);
void insertPTreeNode(ParseTreeNode *prev, ParseTreeNode *ins);

#endif

