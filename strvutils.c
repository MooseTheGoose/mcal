#include "strvutils.h"
#include <glib.h>
#include <ctype.h>

char **strndupv(char **argv, gsize n)
{
    GPtrArray *arr = g_ptr_array_new();
    while(n-- && *argv) g_ptr_array_add(arr, *argv++);
    g_ptr_array_add(arr, NULL);
    return (char **)g_ptr_array_free(arr, FALSE);
}

char **strvstr(char **argv, char *str)
{
    char **loc = NULL;
    char *currStr = *argv;
    
    while(currStr && strcmp(currStr, str)) currStr = *++argv;
    if(currStr) loc = argv;
    
    return loc;
}

char **strvrstr(char **argv, char *str)
{
    char **loc = NULL;
    char *currStr = *argv;
    
    while(currStr)
    {
        if(!strcmp(currStr, str)) { loc = argv; }
        currStr = *++argv;
    }
    
    return loc;
}
