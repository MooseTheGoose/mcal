#include "sutils.h"
#include <glib.h>
#include <ctype.h>

char *substring(const char *first, const char *erase)
{
    int size = erase - first;
    char *sub = g_malloc(size + 1);
    memcpy(sub, first, size);
    sub[size] = '\0';
    return sub;
}

char *trim(const char *str)
{
    const char *strEnd;
    const char *strBegin;
    int currChar = *str;
    
    
    while(isspace(currChar))
    {
        str++;
        currChar = *str;
    }
    
    strBegin = strEnd = str;
    
    while(currChar)
    {
        if(!isspace(currChar)) strEnd = str;
        str++;
        currChar = *str;
    }
    
    return substring(strBegin, strEnd + 1);
}

int isspaceornull(int c)
{
    return isspace(c) || !c; 
}

