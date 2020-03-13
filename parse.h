#ifndef PARSE_H 
#define PARSE_H

#include <glib.h>
#include <string.h>
#include "variables.h"
#include <stdio.h>

/* Actual parsing functions */

int parseStart(GNode *tree, char *expr);
int parseExop(GNode *tree, char *exop);
int parseExp(GNode *tree, char *exp);
int parseMulop(GNode *tree, char *mulop);
int parseMul(GNode *tree, char *mul);
int parseAdop(GNode *tree, char *adop);
int parseAdd(GNode *tree, char *add);
int parseNum(GNode *tree, char *numStart);
double decodeNumber(char *num);

void destroyParseTree(GNode *root);
void cleanOnFail(GNode *root, int status);
void outputParseTree(GNode *root, FILE *xml, int indent);

#endif

