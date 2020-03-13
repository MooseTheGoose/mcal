#include "decoder.h"
#include "error.h"
#include <math.h>

double decodeStart(GNode *parseTree)
{
    GNode *currChild = parseTree->children;
    char *childData = (char *)currChild->data;
    double value;

    if(!strcmp(childData, "("))
    {
        value = decodeStart(currChild->next);
    }
    else if(!strcmp(childData, "NUMBER"))
    {
        value = decodeNumber(currChild);
    }
    else if(!strcmp(childData, "UNARYMINUS"))
    {
        value = decodeUnaryMinus(currChild);
    }
    else if(!strcmp(childData, "EXP"))
    {
        value = decodeExp(currChild);
    }
    else if(!strcmp(childData, "MULT"))
    {
        value = decodeMult(currChild);
    }
    else if(!strcmp(childData, "ADD"))
    {
        value = decodeAdd(currChild);
    }

    return value;
}

double decodeNumber(GNode *parseTree)
{
    return *(double *)parseTree->children->data;
}

double decodeUnaryMinusOp(GNode *parseTree)
{
    GNode *currChild = parseTree->children;
    double value;

    if(!strcmp((char *)currChild->data, "("))
    {
        value = decodeStart(currChild->next);
    }
    else
    {
        value = decodeNumber(currChild);
    }

    return value;
}

double decodeUnaryMinus(GNode *parseTree)
{
    GNode *currChild = parseTree->children->next;
    
    return -decodeUnaryMinusOp(currChild);
}

double decodeExpOp(GNode *parseTree)
{
    GNode *currChild = parseTree->children;
    double value;

    if(!strcmp((char *)currChild->data, "UNARYMINUS"))
    {
        value = decodeUnaryMinus(currChild);
    }
    else
    {
        value = decodeUnaryMinusOp(currChild);
    }

    return value;
}

double decodeExp(GNode *parseTree)
{
    GNode *currChild = parseTree->children;
    double op1, op2;

    if(!strcmp((char *)currChild->data, "EXP"))
    {
        op1 = decodeExp(currChild);
    }
    else
    {
        op1 = decodeExpOp(currChild);
    }

    op2 = decodeExpOp(currChild->next->next);
    return pow(op1, op2);
}

double decodeMultOp(GNode *parseTree)
{
    GNode *currChild = parseTree->children;
    double value;

    if(!strcmp((char *)currChild->data, "EXP"))
    {
        value = decodeExp(currChild);
    }
    else
    {
        value = decodeExpOp(currChild);
    }

    return value;
}

double decodeMult(GNode *parseTree)
{
    GNode *currChild = parseTree->children;
    double op1, op2;
    double value;

    if(!strcmp((char *)currChild->data, "MULT"))
    {
        op1 = decodeMult(currChild);
    }
    else
    {
        op1 = decodeMultOp(currChild);
    }
    
    currChild = currChild->next;
    op2 = decodeMultOp(currChild->next);

    if(!strcmp((char *)currChild->data, "*"))
    {
        value = op1 * op2;
    }
    else
    {
        value = op1 / op2;
    }

    return value;
}

double decodeAddOp(GNode *parseTree)
{
    GNode *currChild = parseTree->children;
    double value;

    if(!strcmp((char *)currChild->data, "MULT"))
    {
        value = decodeMult(currChild);
    }
    else
    {
        value = decodeMultOp(currChild);
    }

    return value;
}

double decodeAdd(GNode *parseTree)
{
    GNode *currChild = parseTree->children;
    double op1, op2;
    double value;

    if(!strcmp((char *)currChild->data, "ADD"))
    {
        op1 = decodeAdd(currChild);
    }
    else
    {
        op1 = decodeAddOp(currChild);
    }

    currChild = currChild->next;
    op2 = decodeAddOp(currChild->next);

    if(!strcmp((char *)currChild->data, "+"))
    {
        value = op1 + op2;
    }
    else
    {
        value = op1 - op2;
    }

    return value;
}

