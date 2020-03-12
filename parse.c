#include "parse.h"
#include <ctype.h>
#include <assert.h>

double parseNum(char *num)
{
    double integ = 0.0;
    double frac = 0.0;
    double div = 1.0;
    int currDigit = *num;
    
    assert(isdigit(currDigit));

    do
    {
        integ *= 10.0;
        integ += (double)(currDigit - '0');
        currDigit = *++num;
    }
    while(isdigit(currDigit));

    if(currDigit == '.')
    {
        currDigit = *++num;
        assert(isdigit(currDigit));
        do
        {
            frac *= 10.0;
            div *= 10.0;
            frac += (double)(currDigit - '0');
            currDigit = *++num;
        }
        while(isdigit(currDigit));
        assert(!currDigit);
        frac /= div;
    }
   
    return integ + frac;
}

