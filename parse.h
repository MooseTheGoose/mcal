#ifndef PARSE_H
#define PARSE_H

#include <glib.h>
#include <string.h>
#include "variables.h"

/* Error utility. */
void dieWhen(int condition, char *lastWords);

/* String Utilities */
char *substring(const char *left, const char *right);

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

#endif

