#ifndef PARSE_H 
#define PARSE_H

#include <glib.h>
#include <string.h>
#include <stdio.h>

/* Actual parsing functions */

GNode *deriveStart(char **tokens);
GNode *deriveUnaryMinusOp(char **tokens);
GNode *deriveUnaryMinus(char **tokens);
GNode *deriveExpOp(char **tokens);
GNode *deriveExp(char **tokens);
GNode *deriveMultop(char **tokens);
GNode *deriveMult(char **tokens);
GNode *deriveAdop(char **tokens);
GNode *deriveAdd(char **tokens);
GNode *deriveNum(char **tokens);

void destroyParseTree(GNode *root);
void outputParseTree(GNode *root, FILE *xml, int indent);

#endif

