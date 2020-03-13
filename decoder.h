#ifndef DECODER_H
#define DECODER_H

#include <glib.h>

double decodeStart(GNode *parseTree);
double decodeUnaryMinusOp(GNode *parseTree);
double decodeUnaryMinus(GNode *parseTree);
double decodeExp(GNode *parseTree);
double decodeExpOp(GNode *parseTree);
double decodeAdd(GNode *parseTree);
double decodeAddOp(GNode *parseTree);
double decodeMult(GNode *parseTree);
double decodeMultOp(GNode *parseTree);
double decodeNumber(GNode *parseTree);

#endif

